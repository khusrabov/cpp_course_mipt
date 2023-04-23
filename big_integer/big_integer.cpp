#include "big_integer.hpp"

// constructors
BigInt::BigInt() : value_{0} {}
BigInt::BigInt(const int64_t kNum) : BigInt(std::to_string(kNum)) {}
BigInt::BigInt(const std::string& str) {
  if (str.empty()) {
    value_ = {0};
    return;
  }

  int start_pos = 0;
  if (str[start_pos] == '-') {
    ++start_pos;
    is_negative_ = true;
  }

  size_t next_pos = str.size() - start_pos - 1;
  value_.resize(str.size() - start_pos);
  for (size_t i = start_pos; i < str.size(); ++i) {
    int elem = str[i] - '0';
    value_[next_pos] = elem;
    --next_pos;
  }
  if (value_.back() == 0) {
    is_negative_ = false;
  }
}

BigInt::BigInt(const BigInt& copy) {
  is_negative_ = copy.is_negative_;
  value_ = copy.value_;
}
BigInt::~BigInt() = default;

// assignment operator
BigInt& BigInt::operator=(const BigInt& other) = default;

// overloading "+"...

// left operand is bigger than right for AdditionOfPositive
BigInt AdditionOfPositive(const BigInt& left_sum, const BigInt& right_sum) {
  BigInt res(left_sum);

  int t = 0;
  int sum;
  for (size_t i = 0; i < res.value_.size(); ++i) {
    sum = res.value_[i] + t;
    if (i < right_sum.value_.size()) {
      sum += right_sum.value_[i];
    }
    res.value_[i] = sum % BigInt::kBase;
    t = sum / BigInt::kBase;
  }
  if (t > 0) {
    res.value_.push_back(t);
  }

  return res;
}

BigInt& BigInt::operator+=(const BigInt& other) {
  *this = *this + other;
  return *this;
}

BigInt operator+(const BigInt& left_sum, const BigInt& right_sum) {
  if (left_sum.is_negative_ && right_sum.is_negative_) {
    return -(Absolute(left_sum) + Absolute(right_sum));
  }
  if (left_sum.is_negative_) {
    return right_sum - Absolute(left_sum);
  }
  if (right_sum.is_negative_) {
    return left_sum - Absolute(right_sum);
  }
  if (left_sum < right_sum) {
    return right_sum + left_sum;
  }
  return AdditionOfPositive(left_sum, right_sum);
}
// overloading "-"...

// left operand is bigger than right for SubstractionOfPositive
BigInt SubstractionOfPositive(const BigInt& left, const BigInt& right) {
  BigInt res(left);

  int c = 0;
  int sub;

  for (size_t i = 0; i < res.value_.size(); ++i) {
    sub = res.value_[i] - c;
    c = 0;
    if (i < right.value_.size()) {
      sub -= right.value_[i];
    }
    if (sub < 0) {
      sub += BigInt::kBase;
      c = 1;
    }
    res.value_[i] = sub;
  }
  res.Normalize();
  return res;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  *this = *this - other;
  return *this;
}

BigInt operator-(const BigInt& left, const BigInt& right) {
  if (left.is_negative_ && right.is_negative_) {
    return -(Absolute(left) - Absolute(right));
  }
  if (left.is_negative_) {
    return -(Absolute(left) + right);
  }
  if (right.is_negative_) {
    return left + Absolute(right);
  }
  if (right > left) {
    return -(right - left);
  }
  return SubstractionOfPositive(left, right);
}

// overloading "*"...
BigInt& BigInt::operator*=(const BigInt& mul) {
  *this = *this * mul;
  return *this;
}
BigInt operator*(const BigInt& left_mul, const BigInt& right_mul) {
  BigInt res;
  if ((left_mul.is_negative_ && !right_mul.is_negative_) ||
      (!left_mul.is_negative_ && right_mul.is_negative_)) {
    res.is_negative_ = true;
  }

  res.value_.resize(left_mul.value_.size() + right_mul.value_.size(), 0);
  for (size_t i = 0; i < left_mul.value_.size(); ++i) {
    for (size_t j = 0; j < right_mul.value_.size(); ++j) {
      res.value_[i + j] += left_mul.value_[i] * right_mul.value_[j];
    }
  }

  int t = 0;
  int cur;
  for (size_t i = 0; i < res.value_.size(); ++i) {
    cur = res.value_[i] + t;
    t = cur / BigInt::kBase;
    res.value_[i] = cur % BigInt::kBase;
  }
  res.Normalize();
  return res;
}

// overloading "/"...
BigInt& BigInt::operator/=(const BigInt& other) {
  *this = *this / other;
  return *this;
}
BigInt operator/(const BigInt& dividend, const BigInt& divisor) {
  BigInt res;
  if ((dividend.is_negative_ && !divisor.is_negative_) ||
      (!dividend.is_negative_ && divisor.is_negative_)) {
    res.is_negative_ = true;
  }

  BigInt temp_dividend;
  BigInt temp_r = 0;
  int next_digit;
  res.value_.resize(dividend.value_.size(), 0);
  for (size_t i = 0; i < dividend.value_.size(); ++i) {
    temp_dividend = temp_r * BigInt::kBase +
                    dividend.value_[dividend.value_.size() - 1 - i];

    for (int j = 0; j < BigInt::kBase; ++j) {
      if (j * Absolute(divisor) > temp_dividend) {
        break;
      }
      next_digit = j;
    }
    temp_r = temp_dividend - next_digit * Absolute(divisor);
    res.value_[dividend.value_.size() - 1 - i] = next_digit;
  }
  res.Normalize();
  return res;
}

// overloading "%"...
BigInt& BigInt::operator%=(const BigInt& nums) {
  *this = *this % nums;
  return *this;
}
BigInt operator%(const BigInt& left, const BigInt& right) {
  return left - left / right * right;
}

// comparison operators "<" "==" ">" "<=" ">=" "!="

bool operator<(const BigInt& left, const BigInt& right) {
  if (left.is_negative_ != right.is_negative_) {
    return left.is_negative_;
  }
  if (left.value_.size() != right.value_.size()) {
    return (left.value_.size() < right.value_.size() && !left.is_negative_) ||
           (left.value_.size() > right.value_.size() && right.is_negative_);
  }
  size_t i = left.value_.size() - 1;
  while (true) {
    if (left.value_[i] != right.value_[i]) {
      return !left.is_negative_ == (left.value_[i] < right.value_[i]);
    }
    if (i == 0) {
      break;
    }
    --i;
  }
  return false;
}

bool operator==(const BigInt& left, const BigInt& right) {
  if (left.is_negative_ != right.is_negative_) {
    return false;
  }
  if (left.value_.size() != right.value_.size()) {
    return false;
  }
  for (size_t i = 0; i < left.value_.size(); ++i) {
    if (left.value_[i] != right.value_[i]) {
      return false;
    }
  }
  return true;
}

bool operator>(const BigInt& left, const BigInt& right) {
  return !(left < right) && !(left == right);
}

bool operator<=(const BigInt& left, const BigInt& right) {
  return (left < right) || (left == right);
}

bool operator>=(const BigInt& left, const BigInt& right) {
  return !(left < right);
}

bool operator!=(const BigInt& left, const BigInt& right) {
  return !(left == right);
}

// unary minus
BigInt BigInt::operator-() const {
  BigInt temp(*this);
  if (value_.back() != 0) {
    temp.is_negative_ = !temp.is_negative_;
  }
  return temp;
}
// prefix increment
BigInt& BigInt::operator++() {
  *this = *this + 1;
  return *this;
}

// postfix increment
BigInt BigInt::operator++(int) {
  BigInt temp(*this);
  *this = *this + 1;
  return temp;
}

// prefix decrement
BigInt& BigInt::operator--() {
  *this = *this - 1;
  return *this;
}

// postfix decrement
BigInt BigInt::operator--(int) {
  BigInt temp(*this);
  *this = *this - 1;
  return temp;
}

// |BigInt|
BigInt Absolute(const BigInt& big_int) {
  BigInt res(big_int);
  res.is_negative_ = false;
  return res;
}

// overloading output/input operators
std::istream& operator>>(std::istream& in, BigInt& big_int) {
  std::string input;
  in >> input;
  big_int = input;
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInt& big_int) {
  if (big_int.is_negative_) {
    out << '-';
  }
  for (auto itr = big_int.value_.rbegin(); itr != big_int.value_.rend();
       ++itr) {
    out << *itr;
  }
  return out;
}
// removing zeros
void BigInt::Normalize() {
  for (size_t i = value_.size() - 1; i > 0; --i) {
    if (value_[i] != 0) {
      break;
    }
    value_.pop_back();
  }
  if (value_.back() == 0) {
    is_negative_ = false;
  }
}
