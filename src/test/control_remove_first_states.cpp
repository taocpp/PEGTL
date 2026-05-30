// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/control/remove_first_states.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct state_a
   {
      unsigned value = 0;
   };

   struct state_b
   {
      unsigned value = 0;
   };

   struct state_c
   {
      unsigned value = 0;
   };

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< one< 'a' > >
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& /*unused*/, States&... st )
      {
         unsigned n = 50;
         ( ( st.value = ( n *= 2 ) + ( st.value * 10 ) + sizeof...( States ) ), ... );
      }
   };

   template< std::size_t N >
   struct test_control
   {
      template< typename Rule >
      using type = typename remove_first_states_n< N, normal >::template type< Rule >;
   };

   void test_0()
   {
      state_a a;
      state_b b;
      state_c c;
      text_view_input<> in( "a" );

      TAO_PEGTL_TEST_ASSERT( parse< one< 'a' >, test_action, test_control< 0 >::template type >( in, a, b, c ) );
      TAO_PEGTL_TEST_ASSERT( a.value == 103 );
      TAO_PEGTL_TEST_ASSERT( b.value == 203 );
      TAO_PEGTL_TEST_ASSERT( c.value == 403 );
   }

   void test_1()
   {
      state_a a;
      state_b b;
      state_c c;
      text_view_input<> in( "a" );

      TAO_PEGTL_TEST_ASSERT( parse< one< 'a' >, test_action, test_control< 1 >::template type >( in, a, b, c ) );
      TAO_PEGTL_TEST_ASSERT( a.value == 0 );
      TAO_PEGTL_TEST_ASSERT( b.value == 102 );
      TAO_PEGTL_TEST_ASSERT( c.value == 202 );
   }

   void test_2()
   {
      state_a a;
      state_b b;
      state_c c;
      text_view_input<> in( "a" );

      TAO_PEGTL_TEST_ASSERT( parse< one< 'a' >, test_action, test_control< 2 >::template type >( in, a, b, c ) );
      TAO_PEGTL_TEST_ASSERT( a.value == 0 );
      TAO_PEGTL_TEST_ASSERT( b.value == 0 );
      TAO_PEGTL_TEST_ASSERT( c.value == 101 );
   }

   void unit_test()
   {
      test_0();
      test_1();
      test_2();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
