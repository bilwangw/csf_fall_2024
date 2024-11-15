#include <cassert>
#include "table.h"
#include "exceptions.h"
#include "guard.h"

Table::Table( const std::string &name )
  : m_name( name )
  // TODO: initialize additional member variables
{
  // TODO: implement
}

Table::~Table()
{
  // TODO: implement
}

void Table::lock()
{
  // TODO: implement
  pthread_mutex_lock(&mutex);
}

void Table::unlock()
{
  // TODO: implement
  pthread_mutex_unlock(&mutex);
}

bool Table::trylock()
{
  // TODO: implement
  return (1 + pthread_mutex_trylock(&mutex)); //trylock returns -1 if fails, returns 0 if true, add one to correct behavior
}

void Table::set( const std::string &key, const std::string &value )
{
  // TODO: implement
  table[key] = value;
}

std::string Table::get( const std::string &key )
{
  // TODO: implement
  return table[key];
}

bool Table::has_key( const std::string &key )
{
  // TODO: implement
  return table.count(key);
}

void Table::commit_changes()
{
  // TODO: implement
}

void Table::rollback_changes()
{
  // TODO: implement
}
