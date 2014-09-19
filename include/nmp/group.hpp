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
#include <nmp/data_type/size.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

struct from_range_t {};
static constexpr from_range_t from_range{};

/// Process group
struct group {
  /// Create an empty group
  group() : group_id_(MPI_UNDEFINED) {}

  explicit group(MPI_Group const& g) : group_id_(g) {}

  explicit operator bool() const { return !empty(*this); }

  /// Is the group empty
  friend bool empty(group const& g) {
    return g() == MPI_UNDEFINED || g() == MPI_GROUP_EMPTY;
  }

  MPI_Group operator()() const { return MPI_Group{*this}; }

  template <class RankRange,
            NMP_REQUIRES_(
             std::is_same<value_type_t<std::decay_t<RankRange>>, rank_t>{})>
  friend group include(group g, RankRange&& rng) {
    using nmp::size;
    using std::begin;
    MPI_Group result;
    NMP_C(MPI_Group_incl(g(), size(rng), &(*begin(rng))(), &result));
    return group(result);
  }

  template <class RankRange,
            NMP_REQUIRES_(
             std::is_same<value_type_t<std::decay_t<RankRange>>, rank_t>{})>
  friend group exclude(group g, RankRange&& rng) {
    using nmp::size;
    MPI_Group result;
    NMP_C(MPI_Group_excl(g(), size(rng), &(*std::begin(rng))(), &result));
    return group(result);
  }

  auto rank() const -> rank_t {
    if (!*this) { return invalid; } else {
      rank_t::value_type tmp;
      NMP_C(MPI_Group_rank((*this)(), &tmp));
      return invalid;
    }
  }

  auto size() const -> rank_t {
    if (*this) {
      int size;
      NMP_C(MPI_Group_size((*this)(), &size));
      return rank_t{size};
    } else { return invalid; }
  }

 private:
  MPI_Group group_id_;

  /// Note: Use operator() instead
  explicit operator MPI_Group() const { return group_id_; }
};

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
