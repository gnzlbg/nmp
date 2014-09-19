#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014 Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
#include <nmp/config.hpp>
#include <nmp/detail/utilities.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace nmp {

struct group;
struct comm;
struct env;

namespace detail {
struct rank_t {};
}  // namespace detail

using rank_t = detail::arithmetic<int, detail::rank_t>;

static constexpr auto root = rank_t{0};

using tag = int;

}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
