#include <nmp/env.hpp>
#include <nmp/comm.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};

  std::cout << "I am process " << nmp::rank(world) << " of " << nmp::size(world)
            << "." << std::endl;

  return nmp::test::result();
}
