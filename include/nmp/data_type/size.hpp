#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Eric Niebler.
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Adapted from Eric Niebler's Range-v3 library:
//   https://github.com/ericniebler/range-v3
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file size.hpp
///
/// Implements an extensible (via ADL) polymorphic function object for computing
/// the size of an Iterable or Message.
///
////////////////////////////////////////////////////////////////////////////////
#include <nmp/detail/requires.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace adl_size {

template <typename Rng,
          NMP_REQUIRES_(std::is_same<category_t<iterator_t<Rng>>,
                                     std::random_access_iterator_tag>{})>
constexpr auto size_(Rng&& rng, long) {
  using std::begin;
  using std::end;
  return end(rng) - begin(rng);
}

template <typename Rng, NMP_REQUIRES_(iterable_t<Rng>{})>
auto size_(Rng&& rng, int) {
  return rng.size();
}

template <typename Rng> auto size(Rng&& rng) {
  return size_(std::forward<Rng>(rng), 42);
}

// Built-in arrays
template <typename T, std::size_t N> constexpr std::size_t size(T(&)[N]) {
  return N;
}

template <typename T, std::size_t N> constexpr std::size_t size(T const(&)[N]) {
  return N;
}

}  // namespace adl_size

using adl_size::size;

template <class T> struct has_size {
  template <class U> static auto test(U&& u, long) -> std::false_type;

  template <class U>
  static auto test(U&& u, int) -> decltype(size(u), std::true_type{});

  using type = decltype(test(std::declval<T>(), 42));
};

template <class T> using has_size_t = typename has_size<T>::type;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
