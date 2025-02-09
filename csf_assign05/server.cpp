#include <iostream>
#include <cassert>
#include <memory>
#include "csapp.h"
#include "exceptions.h"
#include "guard.h"
#include "server.h"
#include <algorithm>

Server::Server()
  // TODO: initialize member variables
{
  pthread_mutex_init(&mutex,nullptr);
  // TODO: implement
}

Server::~Server()
{
  // TODO: implement
  if(server_fd >= 0) {
    close(server_fd);
  }
  pthread_mutex_destroy(&mutex);
}

void Server::listen( const std::string &port )
{
  // TODO: implement
  server_fd = Open_listenfd(port.c_str());
  if(server_fd < 0) {
    log_error("Could not open server socket");
  }
}

void Server::server_loop()
{
  // TODO: implement

  // Note that your code to start a worker thread for a newly-connected
  // client might look something like this:
/*
  ClientConnection *client = new ClientConnection( this, client_fd );
  pthread_t thr_id;
  if ( pthread_create( &thr_id, nullptr, client_worker, client ) != 0 )
    log_error( "Could not create client thread" );
*/
  int keep_going = 1;
  while (keep_going) {
    int client_fd = Accept(server_fd, NULL, NULL);
    if(client_fd > 0) {
      ClientConnection *client = new ClientConnection(this, client_fd);
      pthread_t thr_id;
      //the last argument (client) is just a pointer to client that is passed into the clientworker function as an argument
      if (pthread_create( &thr_id, nullptr, client_worker, client ) != 0) {
        log_error("Could not create client thread");
        close(client_fd);
        keep_going = 0;
      }
      pthread_detach(thr_id);
    }
  }
  close(server_fd);
}


void *Server::client_worker( void *arg )
{
  // TODO: implement

  // Assuming that your ClientConnection class has a member function
  // called chat_with_client(), your implementation might look something
  // like this:
/*
  std::unique_ptr<ClientConnection> client( static_cast<ClientConnection *>( arg ) );
  client->chat_with_client();
  return nullptr;
*/
  
  std::unique_ptr<ClientConnection> client( static_cast<ClientConnection *>( arg ) );
  try {
    client->chat_with_client(); // --> to be implemented in clientconnection.cpp
  }
  catch (CommException &e) {
    std::cout << "failure\n";
  } 
  //pthread_detach(pthread_self());
  return nullptr;
}

void Server::log_error( const std::string &what )
{
  std::cerr << "Error: " << what << "\n";
}

// TODO: implement member functions

//returns 0 if there already exists a table with that name, 1 otherwise
bool Server::create_table( const std::string &name ) {
    Guard g(mutex);
    if(find_table(name) == nullptr) { // check if this table already exists
      tableList.push_back(new Table(name));
      return true;
    } else {
      return false;
    }
}

void Server::delete_table( const std::string &name) {
  bool found = false;
  std::vector<Table*>::iterator it;
  for (it = lockedTables.begin(); it != lockedTables.end(); ++it) {
    if((*it)->get_name() == name) {
      found = true;
      break;
    }
  }
  if(found){
    lockedTables.erase(it); 
  }
}
Table *Server::find_table( const std::string &name ) {
    //Guard g(mutex);
    std::vector<Table*>::iterator it;
    for (it = tableList.begin(); it != tableList.end(); ++it) {
      if((*it)->get_name() == name) {
        return *it;
      }
    }
    return nullptr;
}
// might need to implement table lock tracking in client connection, not server
void Server::lock_table(const std::string &name) {
  //Guard g(mutex);
  Table *lockTable = find_table(name);
  if(lockTable != nullptr) {
    lockedTables.push_back(lockTable);
    lockTable->lock();
  }
}

//if trylock fails (returns false) then rollback changes
bool Server::try_lock_table(const std::string &name) {
  //Guard g(mutex);
  Table *to_lock = find_table(name);
  if (to_lock != nullptr) {
    if (to_lock->trylock()) {
      lockedTables.push_back(to_lock);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
void Server::unlock_table(const std::string &name) {
  bool found = false;
  std::vector<Table*>::iterator it;
  for (it = lockedTables.begin(); it != lockedTables.end(); ++it) {
    if((*it)->get_name() == name) {
      (*it)->unlock();
      found = true;
      break;
    }
  }
  if(found){
    lockedTables.erase(it); // remove from iterator
  }
}