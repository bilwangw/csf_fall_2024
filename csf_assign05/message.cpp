#include <set>
#include <map>
#include <regex>
#include <cassert>
#include "message.h"
#include <cctype> 

bool identifier_valid(std::string id);

Message::Message()
  : m_message_type(MessageType::NONE)
{
}

Message::Message( MessageType message_type, std::initializer_list<std::string> args )
  : m_message_type( message_type )
  , m_args( args )
{
}

Message::Message( const Message &other )
  : m_message_type( other.m_message_type )
  , m_args( other.m_args )
{
}

Message::~Message()
{
}

Message &Message::operator=( const Message &rhs )
{
  this->m_message_type = rhs.m_message_type;
  std::vector<std::string> temp;
  if (rhs.m_args.empty()) {
    this->m_args = temp;
    return *this;
  } else {
    this->m_args = rhs.m_args;
  }
  return *this;
}

MessageType Message::get_message_type() const
{
  return m_message_type;
}

void Message::set_message_type(MessageType message_type)
{
  m_message_type = message_type;
}

std::string Message::get_username() const
{
  //first check type, if it is not valid type return an empty string
  if (m_message_type == MessageType::LOGIN) {
    return get_arg(0);
  }
  return "";
  
}

std::string Message::get_table() const
{
  //first check type, if it is not valid type return an empty string
  if (m_message_type == MessageType::CREATE || m_message_type == MessageType::SET || m_message_type == MessageType::GET) {
    return get_arg(0);
  }
  return "";
}

std::string Message::get_key() const
{
  //first check type, if it is not valid type return an empty string
  if (m_message_type == MessageType::SET || m_message_type == MessageType::GET) {
    return get_arg(1);
  }
  return "";
}

std::string Message::get_value() const
{
  //first check type, if it is not valid type return an empty string
  if (m_message_type == MessageType::PUSH || m_message_type == MessageType::DATA) {
    return get_arg(0);
  }
  return "";
}

std::string Message::get_quoted_text() const
{
  //first check type, if it is not valid type return an empty string
  if (m_message_type == MessageType::FAILED || m_message_type == MessageType::ERROR) {
    return get_arg(0);
  }
  return "";
}

void Message::push_arg( const std::string &arg )
{
  m_args.push_back( arg );
}

bool Message::is_valid() const
{
  // check if valid depending on message type 
  if (m_message_type == MessageType::LOGIN || m_message_type == MessageType::CREATE) {
    return get_num_args() == 1 && identifier_valid(get_arg(0));
  } else if (m_message_type == MessageType::SET || m_message_type == MessageType::GET) {
    return get_num_args() == 2 && identifier_valid(get_arg(0)) && identifier_valid(get_arg(1));
  } else if (m_message_type == MessageType::PUSH || m_message_type == MessageType::DATA ) {
    return get_num_args() == 1;
  } else if (m_message_type == MessageType::FAILED || m_message_type == MessageType::ERROR ) {
    return get_num_args() == 1;
  }
  else {
    return get_num_args() == 0;
  }

}

//helper function to ensure identifier follows strict pattern
bool identifier_valid(std::string id) {
  std::regex pattern("^[A-Za-z0-9_]+$");
  return std::regex_match(id, pattern) && std::isalpha(id[0]);
}
