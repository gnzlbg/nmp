#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Eric Niebler.
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Adapted from Eric Niebler's Range-v3 library:
//   https://github.com/ericniebler/range-v3
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file requires.hpp
///
/// Implements a concept-like requires clause using a macro for constraining
/// templates (via SFINAE) and concept checking.
///
////////////////////////////////////////////////////////////////////////////////
#include <type_traits>
#include <nmp/detail/traits.hpp>
////////////////////////////////////////////////////////////////////////////////

#define NMP_PP_CAT_(X, Y) X##Y
#define NMP_PP_CAT(X, Y) NMP_PP_CAT_(X, Y)

#define NMP_REQUIRES_(...)                                               \
  int NMP_PP_CAT(_concept_requires_, __LINE__)                           \
   = 42,                                                                 \
   ::nmp::enable_if_t < (NMP_PP_CAT(_concept_requires_, __LINE__) == 43) \
   || (__VA_ARGS__) > = 0 /**/

#define NMP_REQUIRES(...) template <NMP_REQUIRES_(__VA_ARGS__)> /**/

////////////////////////////////////////////////////////////////////////////////
