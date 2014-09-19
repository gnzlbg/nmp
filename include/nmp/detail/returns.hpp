#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) Dave Abrahams and Eric Niebler
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// See below for the exact details about where this macro comes from.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file returns.hpp
///
/// Implements the NMP_RETURNS macro
///
////////////////////////////////////////////////////////////////////////////////

/// NMP_RETURNS() is used to avoid writing boilerplate
/// "->decltype(x) { return x; }" phrases.
//
/// USAGE: auto function(<arguments>) NMP_RETURNS(<some-expression>);
///
/// Note: we end with a unique typedef so the function can be followed
/// by a semicolon.  If we omit the semicolon, editors get confused and
/// think we haven't completed the function declaration.
///
/// Author: Dave Abrahams, see
/// https://groups.google.com/forum/#!msg/boost-devel-archive/OzJ5Ft3pSnU\
/// /b_Ter9bgNqAJ .
///
/// \todo Eric Niebler discusses how to improve errors messages when combining
/// the NMP_RETURNS macro with SFINAE for expressions here:
/// https://groups.google.com/forum/#!topic/boost-developers-archive\
/// /Ipn1bF24STc%5B1-25-false%5D .
///
#define NMP_RETURNS(...)                                            \
  noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) \
   ->decltype(__VA_ARGS__) {                                        \
    return (__VA_ARGS__);                                           \
  }                                                                 \
  using NMP_RETURNS_CAT(NMP_RETURNS_, __LINE__) = int
// Standard PP concatenation formula
#define NMP_RETURNS_CAT_0(x, y) x##y
#define NMP_RETURNS_CAT(x, y) NMP_RETURNS_CAT_0(x, y)

////////////////////////////////////////////////////////////////////////////////
