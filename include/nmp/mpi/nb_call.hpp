#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file nb_call.hpp
///
/// Wraps an MPI non-blocking call
////////////////////////////////////////////////////////////////////////////////
#include <nmp/mpi/call.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

template <class F, class... Args> void nb_call(F&& f, Args&&... args) {
  MPI_Request request;
  NMP_C(f(std::forward<Args>(args)..., &request));
  MPI_Status status;
  NMP_C(MPI_Wait(&request, &status));
  return;
}

}  // namespace nmp

#define NMP_NBC(F, ...)                                                    \
  nb_call(                                                                 \
   [&](auto... args) { return F(std::forward<decltype(args)>(args)...); }, \
   __VA_ARGS__) /**/

////////////////////////////////////////////////////////////////////////////////
