// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      count_position p1( 23 );
      count_position p2( 42 );
      TAO_PEGTL_TEST_ASSERT( p1 != p2 );
      TAO_PEGTL_TEST_ASSERT( !( p1 == p2 ) );
      TAO_PEGTL_TEST_ASSERT( p1.count == 23 );
      TAO_PEGTL_TEST_ASSERT( p2.count == 42 );
      count_position p3( 19 );
      p1.apply_offset( p3 );
      TAO_PEGTL_TEST_ASSERT( p1 == p2 );
      TAO_PEGTL_TEST_ASSERT( !( p1 != p2 ) );
      TAO_PEGTL_TEST_ASSERT( p1.count == 42 );
      TAO_PEGTL_TEST_ASSERT( p2.count == 42 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
