#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2005, 2006 Douglas Gregor <doug.gregor -at- gmail.com>.
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Adapted from Boost.MPI
//   https://github.com/boostorg/mpi
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
#include <nmp/nmp_fwd.hpp>
#include <nmp/group.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

/// MPI Communicator
struct comm {
  comm() : comm_id_(MPI_COMM_WORLD) {}

  // copyable (duplicating a communicator not implemented yet)
  comm(comm const&) = delete;             // default;
  comm& operator=(comm const&) = delete;  // default;

  // movable
  comm(comm&&) = default;
  comm& operator=(comm&&) = default;

  /// Construct from a \p subgroup of the communicator \p c
  comm(comm const& c, group const& subgroup) {
    MPI_Comm n_;
    NMP_C(MPI_Comm_create(c(), subgroup(), &n_));
    comm_id_ = n_;
  }

  /// Is empty?
  friend bool empty(comm const& c) { return c.comm_id_ == MPI_COMM_NULL; }

  explicit operator bool() const { return !empty(*this); }

  /// Communicators' rank group
  explicit operator group() const {
    MPI_Group g;
    MPI_Comm_group((*this)(), &g);
    return ::nmp::group(g);
  }

  /// Communicators' underlying id
  MPI_Comm const& operator()() const { return comm_id_; }

  /// Size of communicator c
  auto size() const -> rank_t {
    int size_;
    NMP_C(MPI_Comm_size(comm_id_, &size_));
    return rank_t{size_};
  }

 private:
  MPI_Comm comm_id_;

  /// Note: Use operator() instead
  explicit operator MPI_Comm() const { return comm_id_; }
};

/// Current process rank within communicator \p c
auto rank(comm const& c = comm{}) {
  int rank_;
  NMP_C(MPI_Comm_rank(c(), &rank_));
  return rank_t{rank_};
}

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
