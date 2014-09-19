#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Adapted from eladidan's answer to this SO question:
//   http://stackoverflow.com/questions/14489935/implementing-futurethen-
//   equivalent-for-asynchronous-execution-in-c11?lq=1
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file then.hpp
///
/// Implements a continuatin based .then() function without depending on
/// Boost.Thread's future.
///
////////////////////////////////////////////////////////////////////////////////
#include <future>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

namespace detail {

template <typename T, typename Task>
auto handle_void(std::future<T>& f, Task& w) {
  return w(f.get());
}

template <typename Task> auto handle_void(std::future<void>& f, Task& w) {
  f.wait();
  return w();
}

}  // namespace detail

template <typename T, typename Task> auto then(std::future<T> f, Task w) {
  return std::async(
   [](std::future<T> fc, Task wc) { return detail::handle_void(fc, wc); },
   std::move(f), std::move(w));
}

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
