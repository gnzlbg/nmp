#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file invalid.hpp
///
/// Generic and extensible sentinel value for literal types.
///
////////////////////////////////////////////////////////////////////////////////
#include <limits>
#include <type_traits>
#include <nmp/detail/arithmetic_fwd.hpp>
#include <nmp/detail/returns.hpp>
#include <nmp/detail/requires.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace adl_invalid {

template <class T, NMP_REQUIRES_(!std::is_floating_point<T>{})>
constexpr auto invalid_value(T) NMP_RETURNS(std::numeric_limits<T>::max());

template <class T, NMP_REQUIRES_(std::is_floating_point<T>{})>
constexpr auto invalid_value(T) noexcept -> T {
  static_assert(std::numeric_limits<T>::has_quiet_NaN, "no NaN support!");
  return std::numeric_limits<T>::quiet_NaN();
}

template <class T, class B>
constexpr auto invalid_value(arithmetic<T, B>)
 NMP_RETURNS(arithmetic<T, B>{invalid_value(T{})});

template <class T>
constexpr auto make_invalid(T = T{}) NMP_RETURNS(invalid_value(T{}));

/// Polymorphic functor returning an invalid object of type T
///
/// Useful to represent an empty value without using an optional, e.g.,
/// - no nghbr has been found
/// - node id does not exist
///
/// To extend it define your own invalid_value(T) function for your
/// type T such that it can be find via ADL.
struct invalid {
  template <class T> constexpr operator T() const { return make_invalid<T>(); };
};

template <class T>
static constexpr auto valid(const T& o) NMP_RETURNS(o != T{invalid{}});

}  // namespace adl_invalid

static constexpr adl_invalid::invalid invalid{};

using adl_invalid::valid;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
