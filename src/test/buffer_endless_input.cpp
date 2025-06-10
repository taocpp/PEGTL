// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "test.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test_single()
   {
      static_endless_input< void > in( 'a' );

      for( std::size_t i = 0; i < 42; ++i ) {
         TAO_PEGTL_TEST_ASSERT( parse< rep< 3000, one< 'a' > > >( in ) );
         TAO_PEGTL_TEST_ASSERT( parse< discard >( in ) );
      }
   }

   void test_multiple()
   {
      static_endless_input< void > in( "abc" );
      TAO_PEGTL_TEST_ASSERT( parse< not_at< eof > >( in ) );

      for( std::size_t i = 0; i < 42; ++i ) {
         TAO_PEGTL_TEST_ASSERT( parse< rep< 1000, string< 'a', 'b', 'c' > > >( in ) );
         TAO_PEGTL_TEST_ASSERT( parse< discard >( in ) );
      }
   }

   void unit_test()
   {
      test_single();
      test_multiple();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
