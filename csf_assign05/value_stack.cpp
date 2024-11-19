#include "value_stack.h"
#include "exceptions.h"

ValueStack::ValueStack()
  // TODO: initialize member variable(s) (if necessary)
{
}

ValueStack::~ValueStack()
{
}

bool ValueStack::is_empty() const
{
  // TODO: implement
  return stack.size() == 0;
}

void ValueStack::push( const std::string &value )
{
  // TODO: implement
  stack.push_back(value);
}

std::string ValueStack::get_top() const
{
  // TODO: implement
  if (is_empty()) {
    throw OperationException("Stack is empty");
  }
  return stack.back();
}

void ValueStack::pop()
{
  if (is_empty()) {
    throw OperationException("Stack is empty");
  }
  // TODO: implement
  stack.pop_back();
}
