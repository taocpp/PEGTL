// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/action/apply0_to.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      int value = 0;

      void member( const int v ) noexcept
      {
         value = ( value * 10 ) + v;
      }
   };

   struct other_state
   {};

   void free_function( test_target& target, const int v )
   {
      target.value = ( target.value * 10 ) + v;
   }

   struct a
      : one< 'a' >
   {};

   struct b
      : one< 'b' >
   {};

   struct c
      : one< 'c' >
   {};

   struct grammar
      : seq< a, b, c, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< a >
      : apply0_to< &test_target::value, 1 >
   {};

   template<>
   struct test_action< b >
      : apply0_to< &free_function, 2 >
   {};

   template<>
   struct test_action< c >
      : apply0_to< &test_target::member, 3 >
   {};

   void unit_test()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "abc" );

      TAO_PEGTL_TEST_ASSERT( parse< grammar, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 123 );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
