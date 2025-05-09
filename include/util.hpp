#ifndef JLOX_CPP_INCLUDE_UTIL_HPP_
#define JLOX_CPP_INCLUDE_UTIL_HPP_

#include <iostream>

namespace util {

template <typename T> void print(T arg) { std::cout << arg; }

template <typename T, typename... Types>
void print(T first_arg, Types... args) {
  std::cout << first_arg;
  print(args...);
}

template <typename T> void println(T arg) { std::cout << arg << '\n'; }

template <typename T, typename... Types>
void println(T first_arg, Types... args) {
  std::cout << first_arg;
  println(args...);
}

template <typename T> void print_coll(T coll) {
  typename T::const_iterator pos = coll.begin();
  typename T::const_iterator end = coll.end();
  for (; pos != end; ++pos) {
    print(*pos);
  }
}

template <typename T> void print_coll(T coll, char delimiter) {
  typename T::const_iterator pos = coll.begin();
  typename T::const_iterator end = coll.end();
  for (; pos != end; ++pos) {
    print(*pos, delimiter);
  }
}

} // namespace util

#endif // JLOX_CPP_INCLUDE_UTIL_HPP_
