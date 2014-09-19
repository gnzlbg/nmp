#pragma once
////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2014  Gonzalo Brito Gadeschi.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file license.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////
/// \file at.hpp
///
/// AT_ macro.
///
////////////////////////////////////////////////////////////////////////////////
#include <string>
////////////////////////////////////////////////////////////////////////////////

/// Returns a string containing the position where it is defined. That
/// is, " at function _function_name_ in file _file_name_ at line _line_number_
#define AT_                                                                    \
  (std::string(" at function ") + __PRETTY_FUNCTION__ + " in file " + __FILE__ \
   + " at line " + std::to_string(__LINE__)) /**/

////////////////////////////////////////////////////////////////////////////////
