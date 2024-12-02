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
  rio_readinitb( &m_fdbuf, m_client_fd );
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
  int n;
  //should be a loop in which each iteration reads in a request message from client, processes request, and send response back to client
  do { 
    //read in request from the client 
    n = rio_readlineb(&rio, buf, sizeof(buf));
    std::string request = buf;
    //process the request and actually do stuff (do we just use hella if statements for each message type?)
    Message msg;
    MessageType message = msg.get_message_type();
    std::string return_msg;
    MessageSerialization::decode(request, msg);
    switch(message) { // check which message type and act accordingly
      case MessageType::LOGIN:
        
        break;
      case MessageType::CREATE:
        m_server->create_table(msg.get_table());
        return_msg = "OK\n";
        rio_writen(m_client_fd, return_msg.c_str(), strlen(return_msg.c_str()));
        break;
      case MessageType::PUSH:

        break;
      case MessageType::POP:

        break;
      case MessageType::TOP:

        break;
      case MessageType::SET:

        break;
      case MessageType::GET:

        break;
      case MessageType::ADD:

        break;
      case MessageType::MUL:

        break;
      case MessageType::SUB:
        
        break;
      case MessageType::DIV:

        break;
      case MessageType::BEGIN:

        break;
      case MessageType::COMMIT:

        break;
      case MessageType::BYE:

        break;
      case MessageType::OK:

        break;
      case MessageType::FAILED:

        break;
      case MessageType::ERROR:

        break;
      case MessageType::DATA:

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
