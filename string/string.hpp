#pragma once
#include <cstring>
#include <iostream>
#include <vector>

class String {
public:
  String();

  String(size_t size, char character);

  String(const char* str);

  String(const String& copy);

  String& operator=(const String& str);

  ~String();

  void Clear();

  void PushBack(char character);

  void PopBack();

  void Resize(size_t new_size);

  void Resize(size_t new_size, char character);

  void Reserve(size_t new_cap);

  void ShrinkToFit();

  void Swap(String& other);

  char& operator[](size_t i);

  const char& operator[](size_t i) const;

  char& Front();

  const char& Front() const;

  char& Back();

  const char& Back() const;

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  char* Data();

  const char* Data() const;

  friend bool operator==(const String& a, const String& b);

  friend bool operator>(const String& a, const String& b);

  friend bool operator<(const String& a, const String& b);

  friend bool operator!=(const String& a, const String& b);

  friend bool operator>=(const String& a, const String& b);

  friend bool operator<=(const String& a, const String& b);

  String& operator+=(const String& other);

  friend String operator+(const String& a, const String& b);

  friend String operator*(const String& str, int n);

  friend String& operator*=(String& str, int n);

  friend std::istream& operator>>(std::istream& in, String& str);

  friend std::ostream& operator<<(std::ostream& out, const String& str);

  std::vector<String> Split(const String& delim = " ") const;

  String Join(const std::vector<String>& strings);

private:
  char* characters_ = nullptr;
  size_t string_size_ = 0;
  size_t capacity_ = 0;
  size_t memory_used_ = 0;
};