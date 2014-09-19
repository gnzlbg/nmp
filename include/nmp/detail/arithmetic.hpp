#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file arithmetic.hpp
///
/// Arithmetic type implementation (without implicit conversions).
///
////////////////////////////////////////////////////////////////////////////////
#include <type_traits>
#include <limits>
#include <algorithm>
#include <nmp/detail/invalid.hpp>
////////////////////////////////////////////////////////////////////////////////
namespace nmp {

namespace detail {

/// Arithmetic type without implicit conversions
template <class T, class B = void> struct arithmetic {
  using value_type = T;
  using type = T;

  /// \name Asignment operators
  ///@{
  constexpr arithmetic() noexcept : value{invalid} {};
  constexpr arithmetic(const arithmetic& other) = default;
  constexpr arithmetic(arithmetic&& other) = default;
  constexpr arithmetic& operator=(const arithmetic& other) = default;
  constexpr arithmetic& operator=(arithmetic&& other) = default;

  constexpr explicit arithmetic(const T& other) noexcept(
   std::is_nothrow_constructible<T>::value)
   : value{other} {}
  template <class U, class V>
  constexpr explicit arithmetic(const arithmetic<U, V>& other) noexcept(
   std::is_nothrow_constructible<T>::value)
   : value(other.value) {}
  constexpr arithmetic& operator=(const T& other) noexcept {
    value = other;
    return *this;
  }
  constexpr arithmetic& operator=(adl_invalid::invalid) noexcept {
    value = invalid;
    return *this;
  }
  ///@}

  /// \name Conversion operators
  ///@{
  explicit constexpr operator T() noexcept { return value; }
  explicit constexpr operator const T() const noexcept { return value; }

  template <class U, class V>
  explicit constexpr operator arithmetic<U, V>() noexcept {
    return value;
  }

  template <class U, class V>
  explicit constexpr operator const arithmetic<U, V>() const noexcept {
    return value;
  }
  ///@}

  /// \name Compound assignment +=, -=, *=, /=
  ///@{
  constexpr arithmetic& operator+=(const arithmetic& other) noexcept {
    value += other.value;
    return *this;
  }
  constexpr arithmetic& operator-=(const arithmetic& other) noexcept {
    value -= other.value;
    return *this;
  }
  constexpr arithmetic& operator+=(const T& other) noexcept {
    value += other;
    return *this;
  }
  constexpr arithmetic& operator-=(const T& other) noexcept {
    value -= other;
    return *this;
  }
  constexpr arithmetic& operator*=(const arithmetic& other) noexcept {
    value *= other.value;
    return *this;
  }
  constexpr arithmetic& operator/=(const arithmetic& other) noexcept {
    value /= other.value;
    return *this;
  }
  ///@}

  /// \name Prefix increment operators ++(),--()
  ///@{
  constexpr arithmetic& operator++() noexcept {
    ++value;
    return *this;
  }
  constexpr arithmetic& operator--() noexcept {
    --value;
    return *this;
  }
  ///@}

  /// \name Postfix increment operators ()++,()--
  ///@{
  constexpr arithmetic operator++(int) noexcept {
    arithmetic tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr arithmetic operator--(int) noexcept {
    arithmetic tmp(*this);
    --(*this);
    return tmp;
  }
  ///@}

  /// \name Access operator
  ///@{
  constexpr T& operator()() & noexcept { return value; }
  constexpr T operator()() && noexcept { return value; }
  constexpr T operator()() const& noexcept { return value; }
  ///@}

  /// Data (wrapped value):
  T value;

  /// \name Comparison operators ==, !=, <, >, <=, >=
  /// \relates arithmetic<T, U>
  ///@{
  constexpr bool operator==(const adl_invalid::invalid) {
    return value == T{invalid};
  }
};
///@}

/// swap
/// \relates arithmetic<T, U>
template <class T, class U>
constexpr void swap(arithmetic<T, U>&& a, arithmetic<T, U>&& b) noexcept {
  using std::swap;
  swap(a.value, b.value);
}

/// \name arithmetic operators +,-,*,/,unary -
/// \relates arithmetic<T, U>
///@{
template <class T, class U>
constexpr arithmetic<T, U> operator+(arithmetic<T, U> a,
                                     const arithmetic<T, U>& b) noexcept {
  return a += b;
}
template <class T, class U>
constexpr arithmetic<T, U> operator-(arithmetic<T, U> a,
                                     const arithmetic<T, U>& b) noexcept {
  return a -= b;
}
template <class T, class U>
constexpr arithmetic<T, U> operator*(arithmetic<T, U> a,
                                     const arithmetic<T, U>& b) noexcept {
  return a *= b;
}
template <class T, class U>
constexpr arithmetic<T, U> operator/(arithmetic<T, U> a,
                                     const arithmetic<T, U>& b) noexcept {
  return a /= b;
}

template <class T, class U>
constexpr arithmetic<T, U> operator-(arithmetic<T, U> const& other) noexcept {
  return arithmetic<T, U>{-other.value};
}
///@}

/// \name Comparison operators ==, !=, <, >, <=, >=
/// \relates arithmetic<T, U>
///@{
template <class T, class U>
constexpr bool operator==(const arithmetic<T, U>& a,
                          const arithmetic<T, U>& b) noexcept {
  return a.value == b.value;
}
template <class T, class U>
constexpr bool operator<(const arithmetic<T, U>& a,
                         const arithmetic<T, U>& b) noexcept {
  return a.value < b.value;
}
template <class T, class U>
constexpr bool operator<=(const arithmetic<T, U>& a,
                          const arithmetic<T, U>& b) noexcept {
  return a < b || a == b;
}
template <class T, class U>
constexpr bool operator!=(const arithmetic<T, U>& a,
                          const arithmetic<T, U>& b) noexcept {
  return !(a == b);
}
template <class T, class U>
constexpr bool operator>(const arithmetic<T, U>& a,
                         const arithmetic<T, U>& b) noexcept {
  return !(a <= b);
}
template <class T, class U>
constexpr bool operator>=(const arithmetic<T, U>& a,
                          const arithmetic<T, U>& b) noexcept {
  return !(a < b);
}

/// \name Pointer arithmetic
///@{

template <class T, class U, class V, NMP_REQUIRES_(std::is_pointer<T>{})>
constexpr auto operator+(T a, const arithmetic<U, V>& i) NMP_RETURNS(a + i());

template <class T, class U, class V, NMP_REQUIRES_(std::is_array<T>{})>
constexpr auto operator+(T a, const arithmetic<U, V>& i) NMP_RETURNS(a + i());

template <class T, class U, class V, NMP_REQUIRES_(std::is_pointer<T>{})>
constexpr auto operator-(T a, const arithmetic<U, V>& i) NMP_RETURNS(a - i());

template <class T, class U, class V, NMP_REQUIRES_(std::is_array<T>{})>
constexpr auto operator-(T a, const arithmetic<U, V>& i) NMP_RETURNS(a - i());

///@}

}  // namespace detail

//////////////////////////////////////////////////////////////////////////////
}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////

/// \name arithmetic types are numeric types
/// \relates arithmetic<T, U>
///@{
namespace std {

template <class T, class B>
class numeric_limits<nmp::arithmetic<T, B>> : public numeric_limits<T> {
 public:
  static constexpr bool is_specialized = true;
};

}  // namespace std
///@}

////////////////////////////////////////////////////////////////////////////////
