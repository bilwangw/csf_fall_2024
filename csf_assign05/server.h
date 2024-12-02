#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
#include "table.h"
#include "client_connection.h"
#include <vector>
#include <iterator>

class Server {
private:
  // TODO: add member variables
  std::vector<Table*> tableList;
  std::vector<Table*> lockedTables;
  // copy constructor and assignment operator are prohibited
  Server( const Server & );
  Server &operator=( const Server & );
  int port;
  int server_fd;
public:
  Server();
  ~Server();

  void listen( const std::string &port );
  void server_loop();

  static void *client_worker( void *arg );

  void log_error( const std::string &what );

  // TODO: add member functions

  // Some suggested member functions:
/*
  void create_table( const std::string &name );
  Table *find_table( const std::string &name );
  void log_error( const std::string &what );
*/
  void create_table( const std::string &name );
  Table *find_table( const std::string &name );
  void lock_table( const std::string &name);
  void unlock_table(const std::string &name);
};


#endif // SERVER_H
