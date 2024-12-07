#include <iostream>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "message_serialization.h"
#include "server.h"
#include "exceptions.h"
#include "client_connection.h"

ClientConnection::ClientConnection( Server *server, int client_fd )
  : m_server( server )
  , m_client_fd( client_fd )
{
  //rio_readinitb( &m_fdbuf, m_client_fd );
}

ClientConnection::~ClientConnection()
{
  // TODO: implement
  m_server = nullptr;

}

void ClientConnection::chat_with_client()
{
  rio_t rio;
  rio_readinitb(&rio, this->m_client_fd);
  char buf[1000];
  ssize_t n;
  std::string failed_msg;
  std::string return_msg;
  std::string table_name;
  std::string table_key;
  bool transaction = 0;
  bool login = false;
  bool lock_success;
  //should be a loop in which each iteration reads in a request message from client, processes request, and send response back to client
  do { 
    //read in request from the client 
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (n < 0) {
      rollback_all_changes();
      close(m_client_fd);
      return;
      //throw CommException("ERROR \"Failed I/O\"\n");
    }
    //use try catch blocks to catch errors that are thrown and release locks 

    std::string request = buf;
    //process the request and actually do stuff (do we just use hella if statements for each message type?)
    Message msg;
    try {
      MessageSerialization::decode(request, msg);
    } catch (InvalidMessage& e) { // Fatal error, close client
      std::string str(e.what());
      failed_msg = "ERROR \"" + str + "\"\n";
      rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
      //should release all locks and exit
      rollback_all_changes();
      close(m_client_fd);
      return;
    }
    MessageType message = msg.get_message_type();
    std::vector<Table*>::iterator it;
    Table* new_table;
    if (message == MessageType::LOGIN) { // check the case if message is login, if not proceed with the others
      if(login == false) { // check for first login
        login = true;
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
      } else { // check if repeated logins attempted
        failed_msg = "ERROR \"LOGIN may only be the first message\"\n";
        rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
        if(transaction) {
          this->rollback_all_changes();
        } else {
          unlock_all();
        }
        close(m_client_fd);
        return;
      }
    }
    else {
      if (login) {
        switch(message) { // check which message type and act accordingly
          case MessageType::CREATE:
            //don't need to consider tables created during transaction
            if(m_server->create_table(msg.get_table())) { // check if table can be created
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
              // if (transaction) {
              //   m_server->lock_table(msg.get_table());
              // }
            }
            else {
              failed_msg = "ERROR \"Table " + msg.get_table() + " already exists\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::PUSH:
            stack.push(msg.get_value());
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            break;
          case MessageType::POP:
            try {
              stack.pop();
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            } catch (OperationException& e) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::TOP:
            try {
              return_msg = "DATA " + stack.get_top() + "\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            } catch (OperationException& e) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::SET:
            table_name = msg.get_table();
            table_key = msg.get_key();
            new_table = m_server->find_table(table_name);
            if (new_table == nullptr) { // if table does not exist
              failed_msg = "FAILED \"Table " + table_name + " does not exist\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            if(transaction) { // if transaction mode is on
              if(!check_lock(new_table)) { // check if table is already locked by this transaction
                if(!new_table->trylock()) {
                   // rollback all changes if lock fails
                  transaction = false;
                  failed_msg = "FAILED \"Unable to access table\"\n";
                  rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                  this->rollback_all_changes();
                  break;
                }
                lockedTables.push_back(new_table);
              }
            }
            else { // if autocommit, just lock
              new_table->lock();
            }
            try { //catch exception if stack is empty
              new_table->set(table_key, stack.get_top());
              stack.pop();
            }
            catch (OperationException& e) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              if(!transaction) { // if autocommit, unlock table after
                new_table->unlock();
              }
              rollback_all_changes();
              break;
            }
            if(!transaction) { // if autocommit, immediately commit changes and unlock
              new_table->commit_changes();
              new_table->unlock();
            }
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            break;
          case MessageType::GET:
            table_name = msg.get_table();
            table_key = msg.get_key();
            new_table = m_server->find_table(table_name);
            if (new_table == nullptr) { // if table does not exist
              failed_msg = "FAILED \"Table " + table_name + " does not exist\"\n";
              rollback_all_changes();
              transaction = false;
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            if(transaction) { // if transaction mode is on
              if(!check_lock(new_table)) { // check if table is already locked by this transaction
                lock_success = new_table->trylock(); // try to lock if table is not already locked by transaction
                if(!lock_success) {
                  this->rollback_all_changes(); // rollback all changes if failed
                  failed_msg = "FAILED \"Unable to access table\"\n";
                  transaction = false;
                  rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                  break;
                }
                lockedTables.push_back(new_table);
              }
            }
            else { // if autocommit, just lock
              new_table->lock();
            }
            try { // attempt to get key
              stack.push(new_table->get(table_key));
            }
            catch (OperationException &e) {
              failed_msg = "FAILED \"Unknown key " + table_key + "\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              if(!transaction) { // if autocommit, unlock table after
                new_table->unlock();
              }
              rollback_all_changes();
              transaction = false;
              break;
            }
            if(!transaction) { // make sure to unlock if autocommit
              new_table->unlock();
            }
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            break;
          case MessageType::ADD:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            try {
              int first = std::stoi(stack.get_top());
              stack.pop();
              int second = std::stoi(stack.get_top());
              stack.pop();
              stack.push(std::to_string(first + second));
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            catch (const std::invalid_argument& e){
              failed_msg = "FAILED \"Top two values aren't numeric\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::MUL:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            try {
              int first = std::stoi(stack.get_top());
              stack.pop();
              int second = std::stoi(stack.get_top());
              stack.pop();
              stack.push(std::to_string(first * second));
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            catch (const std::invalid_argument& e){
              failed_msg = "FAILED \"Top two values aren't numeric\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::SUB:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            try {
              int first = std::stoi(stack.get_top());
              stack.pop();
              int second = std::stoi(stack.get_top());
              stack.pop();
              stack.push(std::to_string(second - first));
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            catch (const std::invalid_argument& e){
              failed_msg = "FAILED \"Top two values aren't numeric\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::DIV:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            try {
              int first = std::stoi(stack.get_top());
              stack.pop();
              int second = std::stoi(stack.get_top());
              stack.pop();
              if (first == 0) {
                failed_msg = "FAILED \"Cannot divide by zero\"\n";
                rollback_all_changes();
                rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                break;
              }
              stack.push(std::to_string(second / first));
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            catch (const std::invalid_argument& e){
              failed_msg = "FAILED \"Top two values aren't numeric\"\n";
              rollback_all_changes();
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::BEGIN:
            // if(!transaction) {
            //   transaction = true;
            //   return_msg = "OK\n";
            //   rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            // }
            if (transaction) {
              failed_msg = "FAILED \"Nested transactions aren't supported\"\n";
              rollback_all_changes();
              transaction = false;
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            transaction = true;
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            // for (it = lockedTables.begin(); it != lockedTables.end(); ++it) { // iterate through local locked tables list
            //   (*it)->unlock();
            // }
            // lockedTables.erase(lockedTables.begin(), lockedTables.end());
            break;
          case MessageType::COMMIT:
            if(transaction) {
              transaction = false;
              for (it = lockedTables.begin(); it != lockedTables.end(); ++it) { // iterate through local locked tables list
                  (*it)->commit_changes();
                  (*it)->unlock();
                  // m_server->unlock_table((*it)->get_name());
              }
              lockedTables.erase(lockedTables.begin(),lockedTables.end());
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            else {
              close(m_client_fd);
              unlock_all();
              return;
            }
            break;
          case MessageType::BYE:
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            if(transaction) {
              this->rollback_all_changes();
            } 
            close(m_client_fd);
            return;
          default:
            if(transaction) {
              this->rollback_all_changes();
            }
            close(m_client_fd);
            return;
            break;
          }
      }
      else { // client not logged in
        failed_msg = "ERROR \"First request must be LOGIN\"\n";
        rollback_all_changes();
        rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
        close(m_client_fd);
        return;
      }
    }
  } while (n > 0);
  return;
}

// TODO: additional member functions
void ClientConnection::rollback_all_changes() {
  //delete tables that have been made in client_connection
  for (std::vector<Table*>::iterator it = lockedTables.begin(); it != lockedTables.end(); it++) {
    (*it)->rollback_changes();
    (*it)->unlock();
    
  }
  lockedTables.erase(lockedTables.begin(), lockedTables.end());
  //this might be a bit cooked bc 

  // for (std::vector<Table*>::iterator it = newTables.begin(); it != newTables.end(); it++) {
  //   m_server->delete_table((*it)->get_name());
  // }
  //newTables.erase(newTables.begin(), newTables.end());
}

// checks if table is already locked by this transaction, return true if yes, 
bool ClientConnection::check_lock(Table* table) { 
  for (std::vector<Table*>::iterator it = lockedTables.begin(); it != lockedTables.end(); it++) {
    if((*it)->get_name() == table->get_name()) {
      return true;
    }
  }
  return false;
}

void ClientConnection::unlock_all() {
  for (std::vector<Table*>::iterator it = lockedTables.begin(); it != lockedTables.end(); it++) {
    (*it)->unlock();
  }
}
