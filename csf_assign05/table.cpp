#include <cassert>
#include "table.h"
#include "exceptions.h"
#include "guard.h"
#include <vector>


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
  // TODO: implement
  if (!has_key(key)) {
      return "";
  } else {
    size_t space_location = table[key].find(' ');
    if (space_location != std::string::npos) {
      //std::cout <<  table[key].substr(space_location, table[key].size()-space_location);
      return table[key].substr(space_location+1, table[key].size()-space_location-1);
    } else {
      return table[key];
    }
  }
}

bool Table::has_key( const std::string &key )
{
  // TODO: implement
  return table.count(key);
}

void Table::commit_changes()
{
  // TODO: implement
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
  // TODO: implement
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
