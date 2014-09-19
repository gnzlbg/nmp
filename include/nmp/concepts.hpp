#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file concepts.hpp
///
/// Implements NMP concepts
////////////////////////////////////////////////////////////////////////////////
#include <type_traits>
#include <nmp/data_type/pointer.hpp>
#include <nmp/data_type/map.hpp>
#include <nmp/data_type/size.hpp>
#include <nmp/data_type/unit_of_size.hpp>
#include <nmp/detail/utilities.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace concepts {

using ::nmp::data_ptr;

/// A message M, has:
/// - a unit of size U,
/// - a size (in U units), and
/// - a pointer to its data (of type U* or U const*)
///
/// \note if U is void, then the message size is in the unit char (bytes).
///
/// \note The unit of size is obtained from the trait unit_of_size_t<M>
/// \note The trait has_size_t is defined from the non-member function
/// size(M)
/// \note The trait has_data_t is defined from the non-member function
/// data(M)
///
template <class M> struct message {
  template <class U> static auto test(U&&, long) -> std::false_type;

  template <class U,
            NMP_REQUIRES_(
             has_size_t<M>{}&& has_data_ptr_t<M>{}&& has_unit_t<M>{}  //&&
             // std::is_same<decltype(data_ptr(std::declval<M>())),
             // std::add_pointer_t<unit_of_size_t<M>>>{}
             )>
  static auto test(U&&, int) -> std::true_type;

  using type = decltype(test(std::declval<M>(), 42));

  using unit = unit_of_size_t<M>;
};

}  // namespace concepts

namespace models {

template <class M> struct message : ::nmp::concepts::message<M>::type {};

}  // namespace models

namespace concepts {

/// A target T of message M, has:
/// - the same unit of size M,
/// - a _writable_ pointer to its data.
template <class T, class M> struct target {
  template <class T_, class M_>
  static auto test(T_&&, M_&&, long) -> std::false_type;

  template <class T_, class M_,
            NMP_REQUIRES_(
             models::message<T_>{}&&
             // is writable if pointer type after removing
             // reference is equal
             std::is_same<data_ptr_t<T_>, std::remove_cv_t<data_ptr_t<T_>>>{}&&
              std::is_same<unit_of_size_t<M_>, unit_of_size_t<T_>>{})>
  static auto test(T_&&, M_&&, int) -> std::true_type;

  using type = decltype(test(std::declval<T>(), std::declval<M>(), 42));
};

}  // namespace concepts

namespace models {

template <class T, class M>
struct target : ::nmp::concepts::target<T, M>::type {};

}  // namespace models

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
