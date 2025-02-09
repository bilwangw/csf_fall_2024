#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <set>
#include "message.h"
#include "csapp.h"
#include "value_stack.h"
#include <vector>

class Server; // forward declaration
class Table; // forward declaration

class ClientConnection {
private:
  Server *m_server;
  int m_client_fd;
  rio_t m_fdbuf;
  ValueStack stack;
  std::vector<Table*> lockedTables; // keep track of tables locked during a transaction
  std::vector<Table*> newTables; // keep track of tables created during a transaction
  bool transaction; // track if client is in transaction mode or not
  // copy constructor and assignment operator are prohibited
  ClientConnection( const ClientConnection & );
  ClientConnection &operator=( const ClientConnection & );

public:
  ClientConnection( Server *server, int client_fd );
  ~ClientConnection();

  void chat_with_client();
  bool check_lock(Table* table);
  void unlock_all();
  // TODO: additional member functions
  void rollback_all_changes();
};

#endif // CLIENT_CONNECTION_H
