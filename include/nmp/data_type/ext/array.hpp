#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file array.hpp
///
/// Array wrapper
///
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace detail {

template <class T> struct array_t {
  using value_type = T;
  T* data_;
  int size_;
  T* data() { return data_; }
  int size() { return size_; }
};

}  // namespace detail

template <class T> auto array(T* t, int s) { return detail::array_t<T>{t, s}; }
template <class T> auto array(T const* t, int s) {
  return detail::array_t<const T>{t, s};
}

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
