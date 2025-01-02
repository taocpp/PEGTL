// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< unsigned Size, apply_mode B, rewind_mode N, typename... Rules >
   struct test_rule
   {
      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      static bool match( ParseInput& in, States&&... st )
      {
         static_assert( A == B, "unexpected apply mode" );
         static_assert( M == N, "unexpected rewind mode" );

         TAO_PEGTL_TEST_ASSERT( in.size() == Size );

         return seq< Rules... >::template match< A, M, Action, Control >( in, st... );
      }
   };

   namespace test1
   {
      bool apply_result;

      struct grammar
         : test_rule< 2, apply_mode::action, rewind_mode::optional, any >
      {};

      template< typename Rule >
      struct apply_bool_action
      {};

      template<>
      struct apply_bool_action< grammar >
      {
         template< typename ActionInput >
         static bool apply( const ActionInput& /*unused*/ )
         {
            return apply_result;
         }
      };

      void apply_bool_true()
      {
         apply_result = true;
         text_view_input< scan::lf > in( "ab" );
         const auto result = parse< grammar, apply_bool_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.peek_char() == 'b' );
      }

      void apply_bool_false()
      {
         apply_result = false;
         text_view_input< scan::lf > in( "ab" );
         const auto result = parse< grammar, apply_bool_action >( in );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( in.peek_char() == 'a' );
      }

      template< typename Rule >
      struct apply0_bool_action
      {};

      template<>
      struct apply0_bool_action< grammar >
      {
         static bool apply0()
         {
            return apply_result;
         }
      };

      void apply0_bool_true()
      {
         apply_result = true;
         text_view_input< scan::lf > in( "ab" );
         const auto result = parse< grammar, apply0_bool_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.peek_char() == 'b' );
      }

      void apply0_bool_false()
      {
         apply_result = false;
         text_view_input< scan::lf > in( "ab" );
         const auto result = parse< grammar, apply0_bool_action >( in );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( in.peek_char() == 'a' );
      }

   }  // namespace test1

   void unit_test()
   {
      test1::apply_bool_true();
      test1::apply_bool_false();
      test1::apply0_bool_true();
      test1::apply0_bool_false();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
