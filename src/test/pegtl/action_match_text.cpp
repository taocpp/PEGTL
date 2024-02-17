// Copyright (c) 2019-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/action/change_action.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct remove_state
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... /*unused*/ )
      {
         return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in );
      }
   };

   // further generic helpers could be build, e.g.
   //
   // - change_control
   // - remove prefix/suffix from input (e.g. remove surrounding quotes)
   // - replace the input completely?
   // - append states
   // - prepend states
   // - ...

   std::size_t global_state = 0;
   const char* global_start = nullptr;

   struct state_one
   {
      std::size_t applications_a = 0;
      std::size_t applications_b = 0;
   };

   // clang-format off
   struct grammar_inner : one< 'a' > {};
   struct grammar_one_c : seq< grammar_inner > {};
   struct grammar_one_b : seq< grammar_inner, grammar_one_c > {};
   struct grammar_one_a : seq< grammar_inner, grammar_one_b, eof > {};
   // clang-format on

   template< typename Rule >
   struct action_one_b
   {};

   template< typename Rule >
   struct action_one_t
   {};

   template< typename Rule >
   struct action_one_a
   {};

   template<>
   struct action_one_b< grammar_one_c >
      : remove_state
   {};

   template<>
   struct action_one_b< grammar_inner >
   {
      // used inside of remove_state
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/ )
      {
         ++global_state;
      }

      // used outside of remove_state
      template< typename ActionInput >
      static void apply( const ActionInput& in, state_one& state )
      {
         ++state.applications_b;
         TAO_PEGTL_TEST_ASSERT( in.current_position().count == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current_position().column == 2 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().count == 2 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().column == 3 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().count == 2 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().column == 3 );
         TAO_PEGTL_TEST_ASSERT( in.current() == global_start + 1 );
      }
   };

   template<>
   struct action_one_t< grammar_one_b >
      : change_action< action_one_b >
   {};

   template<>
   struct action_one_a< grammar_one_b >
      : change_action< action_one_t >
   {};

   template<>
   struct action_one_a< grammar_inner >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state_one& state )
      {
         ++state.applications_a;
         TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );
         TAO_PEGTL_TEST_ASSERT( in.current_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().count == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().current_position().column == 2 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().count == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().line == 1 );
         TAO_PEGTL_TEST_ASSERT( in.input().direct_position().column == 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == global_start );
      }
   };

   void unit_test()
   {
      state_one state;
      text_view_input< scan::lf > in( "aaa" );
      global_start = in.current();
      const bool parse_result = parse< grammar_one_a, action_one_a >( in, state );
      TAO_PEGTL_TEST_ASSERT( parse_result );
      TAO_PEGTL_TEST_ASSERT( state.applications_a == 1 );
      TAO_PEGTL_TEST_ASSERT( state.applications_b == 1 );
      TAO_PEGTL_TEST_ASSERT( global_state == 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
