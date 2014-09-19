#include <nmp/nmp.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};

  nmp::barrier(world).get();

  return nmp::test::result();
}
////////////////////////////////////////////////////////////////////////////////
