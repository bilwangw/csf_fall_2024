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
  bool transaction = 0;
  bool login = false;
  //should be a loop in which each iteration reads in a request message from client, processes request, and send response back to client
  do { 
    //read in request from the client 
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (n < 0) {
      throw CommException("ERROR \"Failed I/O\"\n");
    }
    //use try catch blocks to catch errors that are thrown and release locks 

    std::string request = buf;
    //process the request and actually do stuff (do we just use hella if statements for each message type?)
    Message msg;
    try {
      MessageSerialization::decode(request, msg);
    } catch (InvalidMessage& e) { // Fatal error, close client
      std::cout << "invalid message\n";
      std::string str(e.what());
      failed_msg = "ERROR \"" + str + "\"\n";
      rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
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
        }
        close(m_client_fd);
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
            }
            else {
              failed_msg = "ERROR \"Table " + msg.get_table() + " already exists\"\n";
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
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::TOP:
            try {
              return_msg = "DATA " + stack.get_top() + "\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            } catch (OperationException& e) {
              failed_msg = "FAILED \"stack is empty\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::SET:
            new_table = m_server->find_table(msg.get_table());
            if(new_table != nullptr) { // check if table exists
              if (m_server->try_lock_table(msg.get_table())) {
                if(transaction) {
                  lockedTables.push_back(new_table);
                }
                try { //catch exception if stack is empty
                // set functionality does not seem correct, need to be able to hide changes until commit?
                  new_table->set(msg.get_key(), stack.get_top());
                }
                catch (OperationException& e) {
                  failed_msg = "FAILED \"stack is empty\"\n";
                  rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                  break;
                }
                return_msg = "OK\n";
                rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
                if (!transaction) {
                  new_table->commit_changes();
                  m_server->unlock_table(msg.get_table());
                }
              }
              else { // if transaction mode is on, will need to rollback changes
                if(transaction) {
                  this->rollback_all_changes();
                }
                failed_msg = "FAILED \"Unable to access table\"\n";
                rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                break;
              }
              
            }
            else {
              failed_msg = "FAILED \"Table " + msg.get_table() + " does not exist\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            break;
          case MessageType::GET:
            new_table = m_server->find_table(msg.get_table());
            if (new_table != nullptr) {
              if (m_server->try_lock_table(msg.get_table())) {
                if(transaction) {
                  lockedTables.push_back(new_table);
                }
                try {
                  stack.push(new_table->get(msg.get_key()));
                }
                catch (OperationException &e) {
                  failed_msg = "FAILED \"Unknown key " + msg.get_key() + "\"\n";
                  rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                  break;
                }
                return_msg = "OK\n";
                rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
                if(!transaction) {
                  m_server->unlock_table(msg.get_table());
                }
              }
              else {
                if(transaction) {
                  this->rollback_all_changes();
                }
                failed_msg = "FAILED \"Unable to access table\"\n";
                rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                break;
              }
            }
            else {
              failed_msg = "FAILED \"Table " + msg.get_table() + " does not exist\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            }
            
            break;
          case MessageType::ADD:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
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
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::MUL:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
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
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::SUB:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
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
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::DIV:
            if(stack.size() < 2) {
              failed_msg = "FAILED \"stack is empty\"\n";
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
                rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
                break;
              }
              stack.push(std::to_string(second / first));
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            catch (const std::invalid_argument& e){
              failed_msg = "FAILED \"Top two values aren't numeric\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::BEGIN:
            if(!transaction) {
              transaction = true;
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            else {
              failed_msg = "FAILED \"Nested transactions aren't supported\"\n";
              rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              break;
            }
            break;
          case MessageType::COMMIT:
            if(transaction) {
              transaction = false;
              for (it = lockedTables.begin(); it != lockedTables.end(); ++it) { // iterate through local locked tables list
                  (*it)->commit_changes();
                  m_server->unlock_table((*it)->get_name());
              }
              lockedTables.erase(lockedTables.begin(),lockedTables.end());
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
            else {
              close(m_client_fd);
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
    (*it)->unlock();
    (*it)->rollback_changes();
  }
  lockedTables.erase(lockedTables.begin(), lockedTables.end());
  // for (std::vector<Table*>::iterator it = newTables.begin(); it != newTables.end(); it++) {
  //   m_server->delete_table((*it)->get_name());
  // }
  //newTables.erase(newTables.begin(), newTables.end());
}