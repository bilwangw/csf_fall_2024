#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <pthread.h>


class Table {
private:
  std::string m_name;
  std::map<std::string, std::string> table; // added a map to store data
  pthread_mutex_t mutex;
  // copy constructor and assignment operator are prohibited
  Table( const Table & );
  Table &operator=( const Table & );

public:
  Table( const std::string &name );
  ~Table();
  //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  std::string get_name() const { return m_name; }

  void lock();
  void unlock();
  bool trylock();

  // Note: these functions should only be called while the
  // table's lock is held!
  void set( const std::string &key, const std::string &value );
  bool has_key( const std::string &key );
  std::string get( const std::string &key );
  void commit_changes();
  void rollback_changes();
  pthread_mutex_t get_pthread();
};

#endif // TABLE_H
