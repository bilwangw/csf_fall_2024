#include <cassert>
#include "table.h"
#include "exceptions.h"
#include "guard.h"
#include <vector>
#include <iostream>

Table::Table( const std::string &name )
  : m_name( name )
{
}

Table::~Table()
{
  // default 
}

//attempt to lock table
void Table::lock()
{
  pthread_mutex_lock(&mutex);
}

//attempt to unlock table
void Table::unlock()
{
  pthread_mutex_unlock(&mutex);
}

// check if lock is possible
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
    // changed value is stored a space after original value
    // example: OLD_VALUE NEW_VALUE
    table[key] += " " + value;
  } else { // if it is a new key, store the new value, still with a space before to indicate uncommitted
    table[key] = " " + value;
  }
}

std::string Table::get( const std::string &key )
{
  //return empty string if key not in table
  if (!has_key(key)) {
      return "";
  } else {
    // check if the value has a space, which indicates an uncommitted change
    std::cout << table[key] << "\n";
    size_t space_location = table[key].find(' ');
    if (space_location != std::string::npos) {
      // if there is an uncommitted change, return the value after the whitespace
      return table[key].substr(space_location+1, table[key].size()-space_location-1);
    } else {
      // otherwise, just return the value
      return table[key];
    }
  }
}

bool Table::has_key( const std::string &key )
{
  // an existing key should only appear once in the table
  return table.count(key);
}

void Table::commit_changes()
{
  // commit changes by going through the map and removing characters before the whitespace in value
  // the changes are stored after the whitespace, and original value before the whitespace
  for (std::map<std::string,  std::string>::iterator it = table.begin(); it != table.end(); it++) {
    std::string curr = it->second;
    // check if value has a whitespace, which indicates a change has been made
    size_t space_location = curr.find(' ');
    if (space_location != std::string::npos) {
      table[it->first] = table[it->first].substr(space_location+1, table[it->first].size()-space_location-1);
    }
  }

}

void Table::rollback_changes()
{
  //rollback changes by removing characters after the whitespace
  // store keys to be erased in a vector
  std::vector<std::string> to_erase;
  for (std::map<std::string,  std::string>::iterator it = table.begin(); it != table.end(); it++) {
    std::string curr = it->second;
    //check if value has a whitespace
    size_t space_location = curr.find(' ');
    if (space_location != std::string::npos) {
      if (space_location == 0) {
        // if the key itself is a new key, it will start with a whitespace
        // add key to vector for deletion
        to_erase.push_back(it->first);
      } else {
        // if existing key is changed, remove characters after whitespace in value
        table[it->first] = table[it->first].substr(0, space_location-1);
      }
    }
  }
  // remove keys in to_erase from the table
  for (std::vector<std::string>::iterator it = to_erase.begin(); it != to_erase.end(); it++) {
    table.erase(*it);
  }
}