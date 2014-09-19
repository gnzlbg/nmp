#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
///
/// \file layout.hpp
///
/// Description
///
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace detail {

template <class T> struct layout {
  int mpi_data_type;
  T* data_ptr;
  int size;
};

}  // namespace detail

template <class Message, NMP_REQUIRES_(nmp::models::message<Message>{})>
auto layout(Message&& m) {
  return detail::layout<unit_of_size_t<Message>>{
   mpi_data_type(unit_of_size_t<Message>{}), data_ptr(m),
   static_cast<int>(size(m))};
}

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
