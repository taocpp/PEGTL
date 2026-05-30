// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/action/match_typed_state.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct selected_state
   {
      unsigned value = 0;
   };

   struct other_state
   {
      unsigned value = 0;
   };

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< one< 'a' > >
      : match_typed_state< selected_state >
   {
      template< typename ActionInput, typename State >
      static void apply( const ActionInput& /*unused*/, State& st )
      {
         st.value += 1;
      }
   };

   using grammar = one< 'a' >;

   void unit_test()
   {
      unsigned integer = 0;
      other_state other;
      {
         selected_state selected;
         text_view_input in( "a" );

         TAO_PEGTL_TEST_ASSERT( match_typed_state< selected_state >::match< one< 'a' >, apply_mode::enabled, rewind_mode::optional, test_action, normal >( in, other, selected, integer ) );
         TAO_PEGTL_TEST_ASSERT( selected.value == 1 );
         TAO_PEGTL_TEST_ASSERT( other.value == 0 );
         TAO_PEGTL_TEST_ASSERT( integer == 0 );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         selected_state selected;
         text_view_input in( "a" );

         TAO_PEGTL_TEST_ASSERT( parse< grammar, test_action >( in, selected, integer, other ) );
         TAO_PEGTL_TEST_ASSERT( selected.value == 1 );
         TAO_PEGTL_TEST_ASSERT( other.value == 0 );
         TAO_PEGTL_TEST_ASSERT( integer == 0 );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
