#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file mpi_datatype_map.hpp
///
/// Maps literal types to MPI Datatypes
///
////////////////////////////////////////////////////////////////////////////////
#include <nmp/config.hpp>
////////////////////////////////////////////////////////////////////////////////
namespace nmp {

namespace adl_mpi_data_type {

auto mpi_data_type(char) { return MPI_CHAR; }
auto mpi_data_type(double) { return MPI_DOUBLE; }

}  // namespace adl_mpi_data_type

using adl_mpi_data_type::mpi_data_type;

template <class T> struct has_mpi_data_type {
  template <class U> static auto test(U&&, long) -> std::false_type;

  template <class U>
  static auto test(U&&, int)
   -> decltype(mpi_data_type(std::declval<U>()), std::true_type{});

  using type = decltype(test(std::declval<T>(), 42));
};

template <class T>
using has_mpi_data_type_t = typename has_mpi_data_type<T>::type;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
