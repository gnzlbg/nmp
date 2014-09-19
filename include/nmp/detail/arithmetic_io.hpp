#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file arithmetic_fwd.hpp
///
/// I/O for arithmetic types (opt-in).
///
////////////////////////////////////////////////////////////////////////////////
#include <nmp/detail/arithmetic.hpp>
#include <iosfwd>
#include <string>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace detail {

/// istream operator
template <class C, class CT, class T, class B>
inline auto operator>>(std::basic_istream<C, CT>& i, arithmetic<T, B>& v)
 -> decltype(i >> v()) {
  return i >> v();
}

/// ostream operator
template <class C, class CT, class T, class B>
inline auto operator<<(std::basic_ostream<C, CT>& o, const arithmetic<T, B>& v)
 -> decltype(o << v()) {
  return o << v();
}

/// to_string
template <class T, class U>
inline std::string to_string(const arithmetic<T, U> a) {
  return std::to_string(a());
}

}  // namespace detail

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
