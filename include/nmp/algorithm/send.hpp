#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file send.hpp
///
/// Implements NMP's send message.
///
////////////////////////////////////////////////////////////////////////////////
#include <future>
#include <nmp/comm.hpp>
#include <nmp/concepts.hpp>
#include <nmp/nmp_fwd.hpp>
#include <nmp/data_type/layout.hpp>
////////////////////////////////////////////////////////////////////////////////

#define NBSEND MPI_Isend
//#define NBSEND MPI_Issend

namespace nmp {

template <class Message, NMP_REQUIRES_(nmp::models::message<Message>{})>
auto send(nmp::comm const& c, const nmp::rank_t to, const nmp::tag t,
          Message&& m) {
  auto s = layout(m);

  return std::async([&]() {
    NMP_NBC(NBSEND, s.data_ptr, s.size, s.mpi_data_type, to(), t, c());
    return;
  });
}

}  // namespace nmp

////////////////////////////////////////////////////////////////////////////////
#undef NBSEND
////////////////////////////////////////////////////////////////////////////////
