// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <string>
#include <utility>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      int single = 0;
      std::pair< int, int > pair;
      std::string empty = "before";
      int first = 0;
      int second = 0;
      int outer = 0;
      int inner = 0;

      void set( const int in_first, const int in_second ) noexcept
      {
         first = in_first;
         second = in_second;
      }
   };

   struct a
      : one< 'a' >
   {};

   struct b
      : one< 'b' >
   {};

   struct c
      : one< 'c' >
   {};

   struct direct
      : seq< a, b, c, eof >
   {};

   struct x
      : one< 'x' >
   {};

   struct y
      : one< 'y' >
   {};

   struct nested_rule
      : seq< x, y, eof >
   {};

   struct inner_rule
      : one< 'z' >
   {};

   struct outer_rule
      : seq< inner_rule >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< a >
      : const_to< &test_target::single, 1 >
   {};

   template<>
   struct test_action< b >
      : const_to< &test_target::pair, 2, 3 >
   {};

   template<>
   struct test_action< c >
      : const_to< &test_target::empty >
   {};

   template<>
   struct test_action< nested_rule >
      : multi_to< &test_target::set,
                  const_for< x, 4 >,
                  const_for< y, 5 > >
   {};

   template<>
   struct test_action< outer_rule >
      : const_to< &test_target::outer, 6 >
   {};

   template<>
   struct test_action< inner_rule >
      : const_to< &test_target::inner, 7 >
   {};

   void unit_test()
   {
      test_target target;
      text_view_input< scan::lf > direct_input( "abc" );

      TAO_PEGTL_TEST_ASSERT( parse< direct, test_action >( direct_input, target ) );
      TAO_PEGTL_TEST_ASSERT( direct_input.empty() );
      TAO_PEGTL_TEST_ASSERT( target.single == 1 );
      TAO_PEGTL_TEST_ASSERT( target.pair == std::pair< int, int >( 2, 3 ) );
      TAO_PEGTL_TEST_ASSERT( target.empty.empty() );

      text_view_input< scan::lf > nested_input( "xy" );

      TAO_PEGTL_TEST_ASSERT( parse< nested_rule, test_action >( nested_input, target ) );
      TAO_PEGTL_TEST_ASSERT( nested_input.empty() );
      TAO_PEGTL_TEST_ASSERT( target.first == 4 );
      TAO_PEGTL_TEST_ASSERT( target.second == 5 );

      text_view_input< scan::lf > outer_input( "z" );

      TAO_PEGTL_TEST_ASSERT( parse< outer_rule, test_action >( outer_input, target ) );
      TAO_PEGTL_TEST_ASSERT( outer_input.empty() );
      TAO_PEGTL_TEST_ASSERT( target.outer == 6 );
      TAO_PEGTL_TEST_ASSERT( target.inner == 7 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
