// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ARGV_INPUT_HPP
#define TAO_PEGTL_INTERNAL_ARGV_INPUT_HPP

#include <cstring>

#include "../config.hpp"

#include "input_with_start.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct argv_input
      : input_with_start< view_input< char > >
   {
      argv_input( char** argv, const int argn )
         : input_with_start< view_input< char > >( argv[ argn ], std::strlen( argv[ argn ] ) )
      {}
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
