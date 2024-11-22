#include "value_stack.h"
#include "exceptions.h"

ValueStack::ValueStack()
{
}

ValueStack::~ValueStack()
{
}

bool ValueStack::is_empty() const
{
  return stack.size() == 0;
}

void ValueStack::push( const std::string &value )
{
  stack.push_back(value);
}

std::string ValueStack::get_top() const
{
  // check if empty
  if (is_empty()) {
    throw OperationException("Stack is empty");
  }
  // otherwise, return end of stack vector
  return stack.back();
}

void ValueStack::pop()
{
  // check if empty
  if (is_empty()) {
    throw OperationException("Stack is empty");
  }
  // otherwise, pop end of stack vector
  stack.pop_back();
}
