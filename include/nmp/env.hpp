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
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

bool initialized();
bool finalized();

/// MPI Environment
struct env {
  // non-copyable
  env(env const&) = delete;
  env& operator=(env const&) = delete;

  // moveable
  env(env&&) = default;
  env& operator=(env&&) = default;

  env(int argc, char* argv[]) { initialize_mpi(&argc, &argv); }
  env() { initialize_mpi(NULL, NULL); }

  ~env() {
    if (initialized()) { NMP_C(MPI_Finalize()); }
    NMP_ASSERT(finalized() && "MPI should have been finalized!");
  }

 private:
  void initialize_mpi(int* argc, char*** argv) {
    NMP_ASSERT(!initialized() && "MPI has already been initialized!");
    int thread_support;
    NMP_C(MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &thread_support));
    NMP_ASSERT(thread_support == MPI_THREAD_MULTIPLE
               && "multi-threaded MPI unavailable!");
  }
};

void abort(int error_code) { NMP_C(MPI_Abort(MPI_COMM_WORLD, error_code)); };

/// Has the MPI Environment been initialized?
bool initialized() {
  int result;
  NMP_C(MPI_Initialized(&result));
  return result != 0;
};

/// Has the MPI Environment been finalized?
bool finalized() {
  int result;
  NMP_C(MPI_Finalized(&result));
  return result != 0;
};

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
