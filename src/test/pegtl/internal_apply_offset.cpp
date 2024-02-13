// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/count_position.hpp>
#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/text_position_with_source.hpp>

#include <tao/pegtl/internal/apply_offset.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      {
         text_position pos( 100, 200, 300 );
         const text_position off( 10, 20, 30 );
         internal::apply_offset( pos, off );
         TAO_PEGTL_TEST_ASSERT( test::equal( pos, 109, 200, 330 ) );
      } {
         text_position_with_source pos( "foo", 100, 200, 300 );
         const text_position off( 10, 20, 30 );
         internal::apply_offset( pos, off );
         TAO_PEGTL_TEST_ASSERT( test::equal( pos, 109, 200, 330 ) );
      } {
         count_position pos( 100 );
         const count_position off( 300 );
         internal::apply_offset( pos, off );
         TAO_PEGTL_TEST_ASSERT( pos.count == 400 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
