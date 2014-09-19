#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file data_ptr.hpp
///
/// How to obtain a pointer to a data structure's data.
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace adl_data_ptr {

template <class T> constexpr auto data_ptr_(T&& t, long) { return &t; }
template <class T> constexpr auto data_ptr_(T&& t, int) { return t.data(); }

template <class T> constexpr auto data_ptr(T&& t) {
  return data_ptr_(std::forward<T>(t), 42);
}

// Built-in arrays
template <typename T, std::size_t N> constexpr auto data_ptr(T(&t)[N]) -> T * {
  return t;
}

template <typename T, std::size_t N>
constexpr auto data_ptr(T const(&t)[N]) -> const T * {
  return t;
}

}  // namespace adl_data_ptr

using adl_data_ptr::data_ptr;

template <class T> struct has_data_ptr {
  template <class U> static auto test(U&& u, long) -> std::false_type;

  template <class U>
  static auto test(U&& u, int) -> decltype(data_ptr(u), std::true_type{});

  using type = decltype(test(std::declval<T>(), 42));
};

template <class T> using has_data_ptr_t = typename has_data_ptr<T>::type;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
