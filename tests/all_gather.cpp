#include <nmp/nmp.hpp>
#include <nmp/data_type/ext/array.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

void all_gather_in_place(nmp::comm const& world) {
  static const constexpr int N = 2;
  int ref[2 * N] = {0, 1, 2, 3};

  int buf[2 * N];

  for (int i = 0; i != N; ++i) {
    if (rank(world) == nmp::root) { buf[i] = i; } else {
      buf[N + i] = N + i;
    }
  }

  nmp::all_gather(world, nmp::array((int*)buf, N)).get();

  for (int i = 0; i != 2 * N; ++i) { CHECK(buf[i] == ref[i]); }
}

void all_gather_not_in_place(nmp::comm const& world) {
  static const constexpr int N = 2;
  int ref[2 * N] = {0, 1, 2, 3};
  int recv[2 * N];
  int send[N];

  for (int i = 0; i != N; ++i) {
    if (rank(world) == nmp::root) { send[i] = i; } else {
      send[i] = N + i;
    }
  }

  nmp::all_gather(world, nmp::array((int*)send, N),
                  nmp::array((int*)recv, 2 * N)).get();

  for (int i = 0; i != 2 * N; ++i) { CHECK(recv[i] == ref[i]); }
}

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};

  all_gather_in_place(world);
  all_gather_not_in_place(world);

  return nmp::test::result();
}

////////////////////////////////////////////////////////////////////////////////
