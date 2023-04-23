#include "string.hpp"

String::String() {}

String::String(size_t size, char character) {
  memory_used_ = size + 1;
  capacity_ = size;
  string_size_ = size;
  characters_ = new char[memory_used_];
  std::memset(characters_, character, string_size_);
  characters_[string_size_] = '\0';
}

String::String(const char* str) {
  string_size_ = std::strlen(str);
  capacity_ = string_size_;
  memory_used_ = capacity_ + 1;
  characters_ = new char[memory_used_];
  std::memcpy(characters_, str, string_size_);
  characters_[string_size_] = '\0';
}

String::String(const String& copy) {
  string_size_ = copy.string_size_;
  capacity_ = string_size_;
  memory_used_ = string_size_ + 1;
  characters_ = new char[memory_used_];
  std::memcpy(characters_, copy.Data(), memory_used_);
}

String& String::operator=(const String& str) {
  String temp_string(str);
  Swap(temp_string);

  return *this;
}

String::~String() { delete[] characters_; }

void String::Clear() {
  string_size_ = 0;
  characters_[string_size_] = '\0';
}

void String::PushBack(char character) {
  Resize(string_size_ + 1);
  Back() = character;
}

void String::PopBack() {
  if (Size() == 0) {
    return;
  }
  Resize(string_size_ - 1);
}

void String::Resize(size_t new_size) {
  if (new_size < capacity_) {
    string_size_ = new_size;
    characters_[string_size_] = '\0';
    return;
  }

  memory_used_ = 2 * new_size;
  char* temp = new char[memory_used_];

  if (Data() != nullptr) {
    std::memcpy(temp, Data(), Size() + 1);
  }

  delete[] characters_;
  characters_ = temp;
  string_size_ = new_size;
  capacity_ = memory_used_ - 1;
}

void String::Resize(size_t new_size, char character) {
  size_t old_size = Size();
  Resize(new_size);
  std::memset(Data() + old_size, character, Size() - old_size);
}

void String::Reserve(size_t new_cap) {
  if (new_cap <= capacity_) {
    return;
  }

  capacity_ = new_cap;
  memory_used_ = new_cap + 1;
  char* temp = new char[memory_used_];

  if (Data() != nullptr) {
    std::memcpy(temp, Data(), Size() + 1);
  }
  delete[] characters_;
  characters_ = temp;
}

void String::ShrinkToFit() {
  if (capacity_ == string_size_) {
    return;
  }
  char* temp = new char[string_size_ + 1];
  std::memcpy(temp, Data(), Size() + 1);
  delete[] characters_;
  characters_ = temp;
  capacity_ = string_size_;
  memory_used_ = capacity_ + 1;
}

void String::Swap(String& other) {
  std::swap(characters_, other.characters_);
  std::swap(string_size_, other.string_size_);
  std::swap(capacity_, other.capacity_);
  std::swap(memory_used_, other.memory_used_);
}

char& String::operator[](size_t i) { return characters_[i]; }

const char& String::operator[](size_t i) const { return characters_[i]; }

char& String::Front() { return characters_[0]; }

const char& String::Front() const { return characters_[0]; }

char& String::Back() { return characters_[string_size_ - 1]; }

const char& String::Back() const { return characters_[string_size_ - 1]; }

bool String::Empty() const { return string_size_ == 0; }

size_t String::Size() const { return string_size_; }

size_t String::Capacity() const { return capacity_; }

char* String::Data() { return characters_; }

const char* String::Data() const { return characters_; }

bool operator==(const String& a, const String& b) {
  if (a.Size() != b.Size()) {
    return false;
  }
  bool corr = true;
  for (size_t i = 0; i < a.Size(); i++) {
    if (a[i] != b[i]) {
      corr = false;
      break;
    }
  }
  return corr;
}

bool operator>(const String& a, const String& b) {
  size_t min_size = 0;

  if (a.Size() < b.Size()) {
    min_size = a.Size();
  } else {
    min_size = b.Size();
  }

  bool corr = false;
  for (size_t i = 0; i < min_size; i++) {
    if (a[i] < b[i]) {
      return false;
    }
    if (a[i] > b[i]) {
      corr = true;
    }
  }

  if (!corr) {
    if (a.Size() <= b.Size()) {
      return false;
    }
  }
  return true;
}

bool operator<(const String& a, const String& b) {
  if (a > b) {
    return false;
  }
  if (a == b) {
    return false;
  }
  return !(a > b) && !(a == b);
}

bool operator!=(const String& a, const String& b) { return !(a == b); }

bool operator>=(const String& a, const String& b) { return !(a < b); }

bool operator<=(const String& a, const String& b) { return !(a > b); }

String& String::operator+=(const String& other) {
  size_t old_size = Size();
  Resize(string_size_ + other.Size());
  std::memcpy(Data() + old_size, other.Data(), other.Size());
  return *this;
}

String operator+(const String& a, const String& b) {
  String sum;
  size_t size = (a.Size() + b.Size());
  sum.Resize(size);
  const auto* temp = a.Data();
  for (size_t i = 0; i < a.Size(); i++) {
    sum[i] = temp[i];
  }
  temp = b.Data();
  int current = 0;
  for (size_t i = a.Size(); i < size; i++) {
    sum[i] = temp[current];
    current++;
  }

  return sum;
}

String operator*(const String& str, int n) {
  size_t new_size = str.Size() * n;
  String temp;
  temp.Resize(new_size);
  int current = 0;
  for (int i = 0; i < n; i++) {
    for (size_t j = 0; j < str.Size(); j++) {
      temp[current] = str[j];
      current++;
    }
  }
  temp[new_size] = '\0';

  return {temp};
}

String& operator*=(String& str, int n) {
  size_t old_size = str.Size();
  str.Resize(str.Size() * n);
  for (int i = 1; i < n; ++i) {
    memcpy(str.Data() + i * old_size, str.Data(), old_size);
  }
  return str;
}

std::istream& operator>>(std::istream& in, String& str) {
  const size_t kMaxPossible = 1000;
  char input_arr[kMaxPossible];
  in >> input_arr;
  String temp(input_arr);
  str.Swap(temp);
  return in;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  for (size_t i = 0; i < str.Size(); i++) {
    out << str[i];
  }
  return out;
}

String String::Join(const std::vector<String>& strings) {
  size_t size_of_temp = (strings.size() - 1) * Size();
  for (const auto& elem : strings) {
    size_of_temp += elem.Size();
  }

  String temp;
  if (strings.empty()) {
    return temp;
  }
  temp.Resize(size_of_temp);
  size_t current = 0;

  for (size_t i = 0; i < strings[0].Size(); ++i) {
    temp[current] = strings[0][i];
    ++current;
  }
  for (size_t i = 0; i < strings.size(); ++i) {
    for (size_t j = 0; j < Size(); ++j) {
      temp[current] = characters_[j];
      ++current;
    }
    for (size_t k = 0; k < strings[i].Size(); ++k) {
      temp[current] = strings[i][k];
      ++current;
    }
  }

  return temp;
}

std::vector<String> String::Split(const String& delim /*= " "*/) const {
  std::vector<String> split_elems;
  String next_split_elem("");

  size_t it = 0;
  while (Size() - it >= delim.Size()) {
    if (strncmp(&characters_[it], delim.Data(), delim.Size()) != 0) {
      next_split_elem.PushBack(characters_[it]);
      ++it;
      continue;
    }
    split_elems.push_back(next_split_elem);
    next_split_elem.Clear();
    it += delim.Size();
  }
  while (it < Size()) {
    next_split_elem.PushBack(characters_[it]);
    ++it;
  }
  split_elems.push_back(next_split_elem);

  return split_elems;
}