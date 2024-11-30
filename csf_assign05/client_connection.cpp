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
  Server = nullptr;

}

void ClientConnection::chat_with_client()
{
  rio_t rio;
  rio_readinitb(&rio, this->client_fd);
  char buf[1000];
  int n;
  //should be a loop in which each iteration reads in a request message from client, processes request, and send response back to client
  do { 
    //read in request from the client 
    n = rio_readlineb(&rio, buf, sizeof(buf));
    std::string request = buf;
    //process the request and actually do stuff (do we just use hella if statements for each message type?)

  } while (n > 0);
}

// TODO: additional member functions
