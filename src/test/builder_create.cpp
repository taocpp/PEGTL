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

#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct child
   {
      int value = 0;

      bool operator==( const child& ) const = default;
   };

   struct test_target
   {
      child single;
      std::vector< child > children;
   };

   struct other_state
   {};

   struct number
      : plus< digit >
   {};

   struct element
      : seq< one< '[' >, number, one< ']' > >
   {};

   struct elements
      : list< element, one< ',' > >
   {};

   struct bad_element
      : seq< one< '{' >, number, one< '}' > >
   {};

   template< typename Rule >
   struct child_action
      : nothing< Rule >
   {};

   template<>
   struct child_action< number >
      : value_to< &child::value >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< element >
      : create_to< &test_target::single, child, child_action >
   {};

   template<>
   struct test_action< elements >
      : repeat_to< &test_target::children,
                   create_for< element, child, child_action > >
   {};

   template<>
   struct test_action< bad_element >
      : create_to< &test_target::single, child, child_action >
   {};

   void unit_test()
   {
      other_state other;
      test_target direct;
      text_view_input< scan::lf > direct_input( "[42]" );

      TAO_PEGTL_TEST_ASSERT( parse< element, test_action >( direct_input, other, direct ) );
      TAO_PEGTL_TEST_ASSERT( direct_input.empty() );
      TAO_PEGTL_TEST_ASSERT( direct.single == child{ 42 } );

      test_target nested;
      text_view_input< scan::lf > nested_input( "[1],[2],[3]" );

      TAO_PEGTL_TEST_ASSERT( parse< elements, test_action >( nested_input, other, nested ) );
      TAO_PEGTL_TEST_ASSERT( nested_input.empty() );
      TAO_PEGTL_TEST_ASSERT( nested.children == std::vector< child >( { child{ 1 }, child{ 2 }, child{ 3 } } ) );

      test_target failed;
      failed.single.value = 7;
      text_view_input< scan::lf > bad_input( "{42" );

      TAO_PEGTL_TEST_ASSERT( !parse< bad_element, test_action >( bad_input, other, failed ) );
      TAO_PEGTL_TEST_ASSERT( failed.single == child{ 7 } );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
