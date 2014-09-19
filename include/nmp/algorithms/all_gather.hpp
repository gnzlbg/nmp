#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
///
/// \file all_gather.hpp
///
/// Description
///
////////////////////////////////////////////////////////////////////////////////
#include <future>
#include <nmp/comm.hpp>
#include <nmp/concepts.hpp>
#include <nmp/nmp_fwd.hpp>
#include <nmp/data_type/skeleton.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

template <class Message, NMP_REQUIRES_(nmp::models::message<Message>{})>
auto all_gather(nmp::comm const& c, Message&& m) {
  auto s = skeleton(m);

  return std::async([&]() {
    NMP_NBC(MPI_Iallgather, MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, s.data_ptr,
            s.size, s.mpi_data_type, c());
    return;
  });
}

template <class Message, class Target,
          NMP_REQUIRES_(nmp::models::message<Message>{}&&
                         nmp::models::target<Target, Message>{})>
auto all_gather(nmp::comm const& c, Message&& m, Target&& t) {
  NMP_ASSERT(size(t) / size(c)() == size(m));

  auto s = skeleton(m);
  auto r = skeleton(t);

  return std::async([&]() {
    NMP_NBC(MPI_Iallgather, s.data_ptr, s.size, s.mpi_data_type, r.data_ptr,
            r.size / size(c)(), r.mpi_data_type, c());
    return;
  });
}

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
