// Copyright (C) 2013 Andreas Hehn <hehn@phys.ethz.ch>, ETH Zurich
// Copyright (C) 2014 Gonzalo Brito Gadeschi, RWTH Aachen
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Tests adapted from Boost.MPI https://github.com/boostorg/mpi

#include <nmp/env.hpp>
#include <nmp/comm.hpp>
#include <numeric>
#include <vector>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

void group_test(const nmp::comm& comm) {
  std::vector<nmp::rank_t> grp_a_ranks((size(comm) / nmp::rank_t{2})());
  std::iota(std::begin(grp_a_ranks), std::end(grp_a_ranks), 0);

  nmp::comm part_a(comm, include(nmp::group(comm), grp_a_ranks));
  nmp::comm part_b(comm, exclude(nmp::group(comm), grp_a_ranks));

  if (part_a) {
    std::cout << "comm rank: " << rank(comm)
              << " -> part_a rank:" << rank(part_a) << std::endl;
    CHECK(rank(part_a) == rank(comm));
  }
  if (part_b) {
    std::cout << "comm rank: " << rank(comm)
              << " -> part_b rank:" << rank(part_b) << std::endl;
    CHECK(rank(part_b) == rank(comm) - size(comm) / nmp::rank_t{2});
  }
}

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};
  group_test(world);
  return nmp::test::result();
}
////////////////////////////////////////////////////////////////////////////////
