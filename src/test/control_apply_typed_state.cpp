// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/control/apply_typed_state.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_state
   {
      int value = 0;
   };

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< one< 'a' > >
   {
      static void apply0( test_state& st ) noexcept
      {
         TAO_PEGTL_TEST_ASSERT( st.value == 0 );
         st.value += 1;
      }
   };

   template<>
   struct test_action< one< 'b' > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, test_state& st ) noexcept
      {
         TAO_PEGTL_TEST_ASSERT( st.value == 1 );
         st.value += 2;
      }
   };

   template< typename Rule >
   using test_control = apply_typed_state_n< test_state >::type< Rule >;

   struct test_rule
      : seq< one< 'a' >, one< 'b' >, eof >
   {};

   void unit_test()
   {
      int i = 0;
      test_state st;
      double d = 42.0;
      text_view_input< scan::lf > in( "ab" );
      TAO_PEGTL_TEST_ASSERT( parse< test_rule, test_action, test_control >( in, i, st, d ) );
      TAO_PEGTL_TEST_ASSERT( st.value == 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
