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
#include <nmp/data_types.hpp>
#include <nmp/detail/utilities.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace concepts {

using ::nmp::data_ptr;

/// A message M, has:
/// - a unit of size U,
/// - a size (in U units), and
/// - a pointer to its data (of type U*)
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

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
