#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class BigInt {
public:
  // constructors
  BigInt();
  BigInt(int64_t);
  BigInt(const std::string& str);
  BigInt(const BigInt& copy);
  ~BigInt();

  // assignment operator
  BigInt& operator=(const BigInt& other);

  // overloading "+"

  // left operand is bigger than right for AdditionOfPositive
  friend BigInt AdditionOfPositive(const BigInt&, const BigInt&);
  BigInt& operator+=(const BigInt& other);
  friend BigInt operator+(const BigInt& left_sum, const BigInt& right_sum);

  // overloading "-"

  // left operand is bigger than right for SubstractionOfPositive
  friend BigInt SubstractionOfPositive(const BigInt&, const BigInt&);
  BigInt& operator-=(const BigInt& other);
  friend BigInt operator-(const BigInt& left, const BigInt& right);

  // overloading "*"...
  BigInt& operator*=(const BigInt& mul);
  friend BigInt operator*(const BigInt& left_mul, const BigInt& right_mul);

  // overloading "/"...
  BigInt& operator/=(const BigInt& other);
  friend BigInt operator/(const BigInt& dividend, const BigInt& divisor);

  // overloading "%"...
  BigInt& operator%=(const BigInt& nums);
  friend BigInt operator%(const BigInt& left, const BigInt& right);

  // comparison operators "<" "==" ">" "<=" ">=" "!="

  friend bool operator<(const BigInt& left, const BigInt& right);
  friend bool operator==(const BigInt& left, const BigInt& right);
  friend bool operator>(const BigInt& left, const BigInt& right);
  friend bool operator<=(const BigInt& left, const BigInt& right);
  friend bool operator>=(const BigInt& left, const BigInt& right);
  friend bool operator!=(const BigInt& left, const BigInt& right);

  // overloading "-" unary minus
  BigInt operator-() const;

  // prefix increment
  BigInt& operator++();

  // postfix increment
  BigInt operator++(int);

  // prefix decrement
  BigInt& operator--();

  // postfix decrement
  BigInt operator--(int);

  // |BigInt|
  friend BigInt Absolute(const BigInt&);

  // overloading output/input operators

  friend std::istream& operator>>(std::istream& in, BigInt& big_int);

  friend std::ostream& operator<<(std::ostream& out, const BigInt& big_int);

private:
  std::vector<int> value_;
  bool is_negative_ = false;
  static const int kBase = 10;

  // removing zeros
  void Normalize();
};
