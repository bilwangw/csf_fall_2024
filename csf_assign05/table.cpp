#include <cassert>
#include "table.h"
#include "exceptions.h"
#include "guard.h"
#include <vector>
#include <iostream>

Table::Table( const std::string &name )
  : m_name( name )
  // TODO: initialize additional member variables
{
  // so far no additional member variables needed
}

Table::~Table()
{
  // default 
}

void Table::lock()
{
  pthread_mutex_lock(&mutex);
  if (!trylock()) {
    throw FailedTransaction("Mutex lock error");
  }
}

void Table::unlock()
{
  if (!trylock()) {
    throw FailedTransaction("Mutex lock error");
  }
  pthread_mutex_unlock(&mutex);
}

bool Table::trylock()
{
  return (1 + pthread_mutex_trylock(&mutex)); //trylock returns -1 if fails, returns 0 if true, add one to correct behavior
}

void Table::set( const std::string &key, const std::string &value )
{
  if (has_key(key)) {
    size_t space_location = table[key].find(' ');
    //if it has an unstaged change then overwrite it
    if (space_location != std::string::npos) {
      table[key] = table[key].substr(0, space_location-1); 
    } 
    table[key] += " " + value;
  } else {
    table[key] = " " + value;
  }
}

std::string Table::get( const std::string &key )
{
  if (!has_key(key)) {
      return "";
  } else {
    size_t space_location = table[key].find(' ');
    if (space_location != std::string::npos) {
      return table[key].substr(space_location+1, table[key].size()-space_location-1);
    } else {
      return table[key];
    }
  }
}

bool Table::has_key( const std::string &key )
{
  return table.count(key);
}

void Table::commit_changes()
{
  for (std::map<std::string,  std::string>::iterator it = table.begin(); it != table.end(); it++) {
    std::string curr = it->second;
    size_t space_location = curr.find(' ');
    if (space_location != std::string::npos) {
      table[it->first] = table[it->first].substr(space_location+1, table[it->first].size()-space_location-1);
    }
  }

}

void Table::rollback_changes()
{
  std::vector<std::string> to_erase;
  for (std::map<std::string,  std::string>::iterator it = table.begin(); it != table.end(); it++) {
    std::string curr = it->second;
    size_t space_location = curr.find(' ');
    if (space_location != std::string::npos) {
      if (space_location == 0) {
        to_erase.push_back(it->first);
      } else {
        table[it->first] = table[it->first].substr(0, space_location-1);
      }
    }
  }
  for (std::vector<std::string>::iterator it = to_erase.begin(); it != to_erase.end(); it++) {
    table.erase(*it);
  }
}
