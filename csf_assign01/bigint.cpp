#include <cassert>
#include "bigint.h"

BigInt::BigInt()
  // initialize an empty big int
{
  data.push_back(0);
  sign = false;
}

BigInt::BigInt(uint64_t val, bool negative)
  // Initialize a big int with a single uint64_t value and a sign
{
  sign = negative; 
  data.push_back(val);
}

BigInt::BigInt(std::initializer_list<uint64_t> vals, bool negative): data(vals), sign(negative)
  // TODO: initialize member variables
{
}

BigInt::BigInt(const BigInt &other)
  // Copy constructor for BigInt
{
  sign = other.sign;
  data = other.data;
}


BigInt::~BigInt()
{
}

BigInt &BigInt::operator=(const BigInt &rhs)
{
  // Assignment operator for BigInt
  if (this != &rhs) {
    data = rhs.data;
    sign = rhs.sign;
  }
  return *this;
}

bool BigInt::is_negative() const
{
  // Return whether or not BigInt is negative
  // check if value is zero
  // To-do: make zero check work for arbitrary amount of zeros
  if (data[0] == 0 && data.size() == 1) {
    return false;
  }
  return sign;
}

const std::vector<uint64_t> &BigInt::get_bit_vector() const {
  return data;
}

uint64_t BigInt::get_bits(unsigned index) const
{
  // Get the number at a specified index in BigInt
  if (index >= data.size()) {
    return 0;
  }
  return data[index];
}

BigInt BigInt::operator+(const BigInt &rhs) const
{
  // TODO: implement
  unsigned len;
  BigInt sum;
  // Make a variable to track the smallest data vector size
  if(data.size() > rhs.data.size()) {
    len = rhs.data.size();
    
  }
  else {
    len = data.size();
  }
  for(unsigned i = 0; i < len; i++) {
    sum.data.push_back((!(this->is_negative()) * this->get_bits(i)) + (!(rhs.is_negative()) * rhs.get_bits(i)));
  }

}

BigInt BigInt::operator-(const BigInt &rhs) const
{
  // TODO: implement
  // Hint: a - b could be computed as a + -b
}

BigInt BigInt::operator-() const
{
  // Return the negation of the BigInt
  BigInt minus(*this);
  minus.sign = !(minus.sign);
  return minus;
}

bool BigInt::is_bit_set(unsigned n) const
{
  // get the nth bit (starting from 0) and return whether that bit is 1 or not
  unsigned localIndex = n % (sizeof(uint64_t) * 8); // get the index of the desired bit relative to its uint64_t index
  uint64_t bitSet = get_bits(n / (sizeof(uint64_t) * 8)); // get the uint64_t value that n is in
  unsigned cursor = 0;
  while (cursor < localIndex) { // turn to bits, once it gets to nth bit exit loop
    bitSet = bitSet >> 1;
    cursor++;
  }
  return bitSet % 2; //MIGHT BE OFF BY ONE RN
}

BigInt BigInt::operator<<(unsigned n) const
{
  // TODO: implement
}

BigInt BigInt::operator*(const BigInt &rhs) const
{
  // TODO: implement
}

BigInt BigInt::operator/(const BigInt &rhs) const
{
  // TODO: implement
}

int BigInt::compare(const BigInt &rhs) const
{
  // TODO: implement
}

std::string BigInt::to_hex() const
{ 
  // Convert a BigInt into a hexidecimal string
  // Check if the BigInt is empty
  if (data.empty()) {
    return "0";
  }
  // Check if the only number is zero
  if (data.size() == 1 && data.front() == 0) {
    return "0";
  }
  std::string ans = "";
  // Iterate through the uint64_t vector
  for (std::vector<uint64_t>::const_iterator it = data.begin(); it != data.end(); it++) {
    std::string temp = "";
    uint64_t current = *it;
    int digit;
    while (current > 0) {
      digit = current % 16;
      // Append the appropriate hexadecimal value to the string
      if (digit > 9) {
        temp += ((digit-10) + 'a');
      } else {
        temp += (digit + '0');
      }
      current /= 16;
    }
    // Add zeros if there are no values
    if (it != data.end() - 1) {
      while (temp.length() != 16) {
        temp += '0';
      }
    }
    
    ans += temp;
  }
  // Add a negative sign if negative
  if(this->is_negative()) {
    ans += "-";
  }
  // The hexadecimal string is constructed backwards, so it needs to be reversed
  reverse(ans.begin(),ans.end());
  
  return ans;
}

std::string BigInt::to_dec() const
{
  // TODO: implement
}

