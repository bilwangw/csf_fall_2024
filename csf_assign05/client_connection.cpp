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
    } catch (InvalidMessage& e) {
      std::cout << "invalid message\n";
      std::string str(e.what());
      failed_msg = "ERROR \"" + str + "\"\n";
      rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
      continue;
    }
    MessageType message = msg.get_message_type();
    std::vector<Table*>::iterator it;
    Table* new_table;
    //std::cout << request;
    if (message == MessageType::LOGIN) { // check the case if message is login, if not proceed with the others
      if(login == false) { // check for first login
        std::cout << "logged in\n";
        login = true;
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
      } else { // check if repeated logins attempted
        failed_msg = "ERROR \"LOGIN may only be the first message\"\n";
        rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
      }
    }
    else {
      if (login) {
        switch(message) { // check which message type and act accordingly
        // case MessageType::LOGIN:
        //   break;
        case MessageType::CREATE:
          if (m_server->create_table(msg.get_table())) {
            if (transaction) {
              //try to lock the tables
              if (!m_server->try_lock_table(msg.get_table())) {
                //rollback changes
                this->rollback_all_changes();
                failed_msg = "ERROR \"Unable to commit changes\"\n"; // output when rollback
                rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
              }
              else {
                newTables.push_back(m_server->find_table(msg.get_table())); // add to list of created tables
                lockedTables.push_back(m_server->find_table(msg.get_table()));
                m_server->lock_table(msg.get_table());
                return_msg = "OK\n";
                rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
              }
            }
            else { // if autocommit mode and table does not exist
              return_msg = "OK\n";
              rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            }
          }
          else { // if table already exists, regardless of transaction or autocommit mode
            std::cout << "table exists\n";
            //output error (table already exists)
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
          if (new_table == nullptr) {
            failed_msg = "FAILED \"Table " + msg.get_table() + " does not exist\"\n";
            rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            //handle error --> unsure if we should just back out
            break;
          }
          if(transaction) { // if transaction mode is on, add to local table lock tracker
            if (m_server->try_lock_table(msg.get_table())) {
              lockedTables.push_back(new_table);
            } else {
              //rollback
              this->rollback_all_changes();
            }
          } else {
            m_server->lock_table(msg.get_table());
          }
          // always lock table on the server
          try { //catch exception if stack is empty
            new_table->set(msg.get_key(), stack.get_top());
          }
          catch (OperationException& e) {
            failed_msg = "FAILED \"stack is empty\"\n";
            rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
          }
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
          if (!transaction) { // if autocommit mode is on, unlock immediately after command finishes
            m_server->unlock_table(msg.get_table());
          }
          break;
        case MessageType::GET:
          new_table = m_server->find_table(msg.get_table());
          if (new_table != nullptr) {
            if (transaction) {
              if (m_server->try_lock_table(msg.get_table())) { //try to obtain a lock, if successful, add it to locked tables
                  lockedTables.push_back(new_table);
              } else {
                //rollback changes
                this->rollback_all_changes();
              }
            } else {
              m_server->lock_table(msg.get_table());
            }
            stack.push(new_table->get(msg.get_key()));
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
            if (!transaction) {
              m_server->unlock_table(msg.get_table());
            }
          }
          else { // if table does not exist
            failed_msg = "FAILED \"Table " + msg.get_table() + " does not exist\"\n";
            rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
            break;
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
            throw OperationException("Top two values are not integers\n");
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
            throw OperationException("Top two values are not integers\n");
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
            throw OperationException("Top two values are not integers\n");
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
              throw OperationException("Cannot divide by 0\n");
            }
            stack.push(std::to_string(second / first));
            return_msg = "OK\n";
            rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
          }
          catch (const std::invalid_argument& e){
            throw OperationException("Top two values are not integers\n");
          }
          break;
        case MessageType::BEGIN:
          transaction = true;
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
          break;
        case MessageType::COMMIT:
          transaction = false;
          for (it = lockedTables.begin(); it != lockedTables.end(); ++it) { // iterate through local locked tables list
              (*it)->commit_changes();
              m_server->unlock_table((*it)->get_name());
          }
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
          break;
        case MessageType::BYE:
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
          return;
        default:
          //throw error
          //throw InvalidMessage("Message type has no match");
          // failed_msg = "ERROR \"Unknown message type \"\n";
          // rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
          break;
        }
      }
      else { // client not logged in
        failed_msg = "ERROR \"First request must be LOGIN\"\n";
        rio_writen(m_client_fd, failed_msg.c_str(), strlen(failed_msg.c_str()));
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
  for (std::vector<Table*>::iterator it = newTables.begin(); it != newTables.end(); it++) {
    m_server->delete_table((*it)->get_name());
  }
  newTables.erase(newTables.begin(), newTables.end());
}