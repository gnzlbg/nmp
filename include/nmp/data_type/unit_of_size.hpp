#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file unit_of_size.hpp
///
/// For serialization pourposes one needs not only the size (number of elements)
/// in a sequence but also the size of the elements themselves (the unit of
/// size).
///
////////////////////////////////////////////////////////////////////////////////
#include <nmp/detail/traits.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

template <class T, class = void> struct unit_of_size {};

template <class T> struct unit_of_size<T, void> {
  using type =
   typename std::remove_cv_t<std::remove_reference_t<T>>::value_type;
};

template <class T, int N> struct unit_of_size<T const[N], void> {
  using type = const T;
};

template <class T, int N> struct unit_of_size<T[N], void> { using type = T; };

template <class T, int N> struct unit_of_size<T const(&)[N], void> {
  using type = const T;
};

template <class T, int N> struct unit_of_size<T(&)[N], void> {
  using type = T;
};

template <class T> using unit_of_size_t = typename unit_of_size<T>::type;

template <class T> struct has_unit {
  template <class U> static auto test(U&&, long) -> std::false_type {}

  template <class U>
  static auto test(U&&, int) -> decltype(unit_of_size_t<T>{}, std::true_type{});

  using type = decltype(test(std::declval<T>(), 42));
};

template <class T> using has_unit_t = typename has_unit<T>::type;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
