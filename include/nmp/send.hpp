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
////////////////////////////////////////////////////////////////////////////////

#define NBSEND MPI_Isend
//#define NBSEND MPI_Issend

namespace nmp {

template <class Message, NMP_REQUIRES_(nmp::models::message<Message>{})>
auto send(nmp::comm c, nmp::rank_t to, nmp::tag t, Message&& m) {
  auto mpi_data_type_ = mpi_data_type(unit_of_size_t<Message>{});
  auto data_ptr_ = data_ptr(m);
  auto size_ = size(m);

  return std::async([=]() {
    MPI_Request request;
    NMP_C(NBSEND(data_ptr_, size_, mpi_data_type_, to(), t, c(), &request));
    MPI_Status status;
    NMP_C(MPI_Wait(&request, &status));
    return;
  });
}

}  // namespace nmp

////////////////////////////////////////////////////////////////////////////////
#undef NBSEND
////////////////////////////////////////////////////////////////////////////////
