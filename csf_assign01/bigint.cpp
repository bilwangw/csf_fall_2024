#include <cassert>
#include "bigint.h"
#include <algorithm>

BigInt::BigInt()
  // TODO: initialize member variables
{
  data.push_back(0);
  sign = false;
}

BigInt::BigInt(uint64_t val, bool negative)
  // TODO: initialize member variables
{
  sign = negative; 
  data.push_back(val);
}

BigInt::BigInt(std::initializer_list<uint64_t> vals, bool negative): data(vals), sign(negative)
  // TODO: initialize member variables
{
}

BigInt::BigInt(const BigInt &other)
  // TODO: initialize member variables
{
  sign = other.sign;
  data = other.data;
}

BigInt::~BigInt()
{
}

BigInt &BigInt::operator=(const BigInt &rhs)
{
  // TODO: implement
  if (this != &rhs) {
    data = rhs.data;
    sign = rhs.sign;
  }
  return *this;
}

bool BigInt::is_negative() const
{
  // TODO: implement
  return sign;
}

const std::vector<uint64_t> &BigInt::get_bit_vector() const {
  return data;
}

uint64_t BigInt::get_bits(unsigned index) const
{
  // TODO: implement
  return data[index];
}

BigInt BigInt::operator+(const BigInt &rhs) const
{
  // TODO: implement
}

BigInt BigInt::operator-(const BigInt &rhs) const
{
  // TODO: implement
  // Hint: a - b could be computed as a + -b
}

BigInt BigInt::operator-() const
{
  // TODO: implement
  BigInt minus(*this);
  return minus;
}

bool BigInt::is_bit_set(unsigned n) const
{
  // TODO: implement
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
  if (data.empty()) {
    return "0";
  }
  std::string ans = "";
  for (std::vector<uint64_t>::const_iterator it = data.begin(); it != data.end(); it++) {
    uint64_t current = *it;
    //uint64_t counter = 1;
    int digit;
    while (current != 0) {
      digit = current % 16;
      if (digit > 9) {
        ans += ((digit-10) + 'a');
      } else {
        ans += (digit + '0');
      }
      //counter *= 16;
      current /= 16;
    }
  }
  if(sign) {
    ans += "-";
  }
  reverse(ans.begin(),ans.end());
  //std::cout << ans << "\n";
  
  return ans;
  // TODO: implement
  // divide 1, divide by 16, 
}

std::string BigInt::to_dec() const
{
  // TODO: implement
}

