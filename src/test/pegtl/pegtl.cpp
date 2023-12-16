// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( sizeof( char ) == 1 );
      static_assert( sizeof( char16_t ) == 2 );
      static_assert( sizeof( char32_t ) == 4 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
