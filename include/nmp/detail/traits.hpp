#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Eric Niebler.
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Some of the traits have been adapted from Eric Niebler's Range-v3 library:
//   https://github.com/ericniebler/range-v3
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file traits.hpp
///
/// Implements useful non-standard and domain-specific type traits.
///
////////////////////////////////////////////////////////////////////////////////
#include <iterator>
#include <nmp/detail/requires.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

template <bool Condition, typename T = int>
using enable_if_t = typename std::enable_if<Condition, T>::type;

template <class T> using value_type_t = typename T::value_type;

template <class T> using iterator_t = typename T::iterator;

template <class T>
using category_t = typename std::iterator_traits<T>::iterator_category;

namespace detail {

template <class U> struct is_iterable_ {
  template <class T> static auto test(T&& t, long) -> std::false_type;

  template <class T> static auto test(T&& t, int) -> std::true_type {
    using std::begin;
    using std::end;
    auto b = begin(t);
    auto e = end(t);
  }

  using type = decltype(test(std::declval<U>(), 42));
};

}  // namespace detail

template <class T> using iterable_t = typename detail::is_iterable_<T>::type;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
