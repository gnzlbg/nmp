#include <nmp/nmp.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  auto env = nmp::env{argc, argv};
  auto world = nmp::comm{};

  auto root_tag = 0;
  auto other_tag = 1;

  char inp_buf1[100];
  char inp_buf2[100];

  if (rank(world) == nmp::root) {
    nmp::rank_t to = nmp::rank_t{1};
    const char out_msg1[] = "Hello ";
    const char out_msg2[] = " back from root!";

    // send msg1, and when that completes, send msg2
    auto future_send = nmp::send(world, to, root_tag, out_msg1);
    auto future_send2 = nmp::then(std::move(future_send), [&]() {
      return nmp::send(world, to, root_tag, out_msg2);
    });
    auto future_recv = nmp::recv(world, to, other_tag, inp_buf1);
    auto future_recv2 = nmp::then(std::move(future_recv), [&]() {
      return nmp::recv(world, to, other_tag, inp_buf2);
    });

    future_send2.get();
    future_recv2.get();

    std::cout << "Root rank received: " << inp_buf1 << inp_buf2 << "\n";

  } else {
    nmp::rank_t to = nmp::root;
    const char out_msg1[] = "Aloha ";
    const char out_msg2[] = " back from non-root!";

    // send msg1, and when that completes, send msg2
    auto future_send = nmp::send(world, to, other_tag, out_msg1);
    auto future_send2 = nmp::then(std::move(future_send), [&]() {
      return nmp::send(world, to, other_tag, out_msg2);
    });
    auto future_recv = nmp::recv(world, to, root_tag, inp_buf1);
    auto future_recv2 = nmp::then(std::move(future_recv), [&]() {
      return nmp::recv(world, to, root_tag, inp_buf2);
    });

    future_send2.get();
    future_recv2.get();

    std::cout << "Non-Root rank received: " << inp_buf1 << inp_buf2 << "\n";
  }

  return nmp::test::result();
}

////////////////////////////////////////////////////////////////////////////////
