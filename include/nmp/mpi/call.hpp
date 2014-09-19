#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file call.hpp
///
/// Wraps an MPI call on a macro that handles the error code and terminates or
/// throws the program in case of error (after printing the real MPI error).
////////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <string>
#include <nmp/exceptions.hpp>
#include <nmp/config.hpp>
#include <nmp/detail/at.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

template <class F> void mpi_call(F&& f, std::string at) {
  const auto error_code = f();

#ifdef NMP_TERMINATE_ON_ERROR
  if (error_code != MPI_SUCCESS) {
    char error_string[MPI_MAX_ERROR_STRING];
    int length;
    MPI_Error_string(error_code, error_string, &length);
    fprintf(stderr, "[MPI ERROR]: \n %s \n at: %s \n", error_string,
            at.c_str());
    exit(EXIT_FAILURE);
  }
#elif NMP_THROW_ON_ERROR
  if (error_code != MPI_SUCCESS) {
    auto error_class = MPI_ERROR_CLASS(error_code);
    switch (error_class) {
      case WHATEVER: {
        throw nmp::whatever{};
      }
      default: { throw nmp::unknown_error{}; }
    }
  }
#endif
}

}  // namespace nmp

#define NMP_C(F) mpi_call([&]() { return F; }, AT_)

////////////////////////////////////////////////////////////////////////////////
