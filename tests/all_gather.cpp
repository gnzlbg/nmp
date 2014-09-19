#include <nmp/nmp.hpp>
#include <nmp/data_type/ext/array.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};

  auto root_tag = 0;
  auto other_tag = 1;

  static const constexpr int N = 4;

  int buf[N];
  int ref[N];

  if (rank(world) == nmp::root) {
    nmp::rank_t to = nmp::rank_t{1};

    for (int i = 0; i != N; ++i) {
      if (i < N / 2) { buf[i] = i; } else {
        buf[i] = 0;
      }
      ref[i] = i;
    }

    auto f0 = nmp::all_gather(world, nmp::array((int*)buf, N / 2));
    f0.get();

    for (int i = 0; i != N; ++i) { CHECK(ref[i] == buf[i]); }

  } else {
    nmp::rank_t to = nmp::root;

    for (int i = 0; i != N; ++i) {
      if (i < N / 2) { buf[i] = 0; } else {
        buf[i] = i;
      }
      ref[i] = i;
    }

    auto f0 = nmp::all_gather(world, nmp::array((int*)buf, N / 2));
    f0.get();

    for (int i = 0; i != N; ++i) { CHECK(ref[i] == buf[i]); }
  }

  return nmp::test::result();
}

////////////////////////////////////////////////////////////////////////////////
