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
  //should be a loop in which each iteration reads in a request message from client, processes request, and send response back to client
  do { 
    //read in request from the client 
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (n < 0) {
      m_server->log_error("IO error");
    }
    std::string request = buf;
    //process the request and actually do stuff (do we just use hella if statements for each message type?)
    Message msg;
    MessageSerialization::decode(request, msg);
    MessageType message = msg.get_message_type();
    std::string return_msg;
    std::vector<Table*>::iterator it;
    Table* new_table;
    std::cout << request;
    switch(message) { // check which message type and act accordingly
      case MessageType::LOGIN:
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::CREATE:
        m_server->create_table(msg.get_table());
        if (transaction) { // if transaction mode is on, add table to client and server locked table lists
          newTables.push_back(m_server->find_table(msg.get_table())); // add to list of created tables
          lockedTables.push_back(m_server->find_table(msg.get_table()));
          m_server->lock_table(msg.get_table());
        }
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::PUSH:
        stack.push(msg.get_value());
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::POP:
        stack.pop();
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::TOP:
        return_msg = "DATA " + stack.get_top() + "\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::SET:
        new_table = m_server->find_table(msg.get_table());
        if(transaction) { // if transaction mode is on, add to local table lock tracker
          lockedTables.push_back(new_table);
        }
        m_server->lock_table(msg.get_table()); // always lock table on the server
        if (new_table != nullptr) {
          new_table->set(msg.get_key(), stack.get_top());
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        }
        else {
          m_server->log_error("Invalid table");
        }
        if (!transaction) { // if autocommit mode is on, unlock immediately after command finishes
          m_server->unlock_table(msg.get_table());
        }
        break;
      case MessageType::GET:
        new_table = m_server->find_table(msg.get_table());
        if (new_table != nullptr) {
          stack.push(new_table->get(msg.get_key()));
          return_msg = "OK\n";
          rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        }
        else {
          m_server->log_error("Invalid table");
        }
        break;
      case MessageType::ADD:
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
        break;
      default:
        //throw error
        throw InvalidMessage("Message type has no match");
        break;
    }

  } while (n > 0);
  return;
}

// TODO: additional member functions
