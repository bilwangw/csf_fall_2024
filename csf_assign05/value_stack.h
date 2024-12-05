#ifndef VALUE_STACK_H
#define VALUE_STACK_H

#include <vector>
#include <string>

class ValueStack {
private:
  std::vector<std::string> stack; // use a vector to store value stack

public:
  ValueStack();
  ~ValueStack();

  bool is_empty() const;
  void push( const std::string &value );

  // Note: get_top() and pop() should throw OperationException
  // if called when the stack is empty

  std::string get_top() const;
  size_t size() const;
  void pop();
};

#endif // VALUE_STACK_H
