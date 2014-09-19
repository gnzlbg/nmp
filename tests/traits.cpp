#include <vector>
#include <nmp/concepts.hpp>
#include "test.hpp"
////////////////////////////////////////////////////////////////////////////////

template <class Unit, class Message> bool is_message(Message&& m, int size_) {
  using nmp::data_ptr;
  auto has_unit = nmp::has_unit_t<Message>{};
  auto has_correct_unit
   = static_cast<bool>(std::is_same<nmp::unit_of_size_t<Message>, Unit>{});
  auto has_size = static_cast<bool>(nmp::has_size_t<Message>{});
  auto has_data_ptr = static_cast<bool>(nmp::has_data_ptr_t<Message>{});
  auto correct_data_ptr_unit
   = std::is_same<std::add_pointer_t<Unit>, decltype(data_ptr(m))>::value;

  using nmp::size;
  auto has_correct_size = size(m) == static_cast<std::size_t>(size_);

  auto has_mpi_data_type = static_cast<bool>(
   nmp::has_mpi_data_type_t<nmp::unit_of_size_t<Message>>{});

  return has_unit && has_correct_unit && has_size && has_correct_size
         && has_data_ptr && correct_data_ptr_unit && has_mpi_data_type;
};

int main() {
  const char m1[] = "abcdef";
  char m2[] = "abcdef";
  std::vector<double> m3 = {1, 2, 3};

  CHECK(is_message<const char>(m1, 7));
  CHECK(is_message<char>(m2, 7));
  CHECK(is_message<double>(m3, 3));

  CHECK(typename nmp::models::message<decltype(m1)>{});
  CHECK(typename nmp::models::message<decltype(m2)>{});
  CHECK(typename nmp::models::message<decltype(m3)>{});

  return nmp::test::result();
}

////////////////////////////////////////////////////////////////////////////////
