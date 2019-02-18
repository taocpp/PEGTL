// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/hierarchical.hpp>

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/rules.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      struct state_one
      {
         int byte_in_line_a;
         int byte_in_line_b;
      };

      // clang-format off
      struct grammar_inner : one< 'a' > {};
      struct grammar_one_b : seq< grammar_inner > {};
      struct grammar_one_a : seq< grammar_inner, grammar_one_b, eof > {};

      template< typename Rule >
      struct action_one_b : nothing< Rule > {};

      template< typename Rule >
      struct action_one_a : nothing< Rule > {};
      // clang-format on

      template<>
      struct action_one_b< grammar_inner >
      {
         template< typename Input >
         static void apply( const Input& in, state_one& state )
         {
            state.byte_in_line_b = in.input().byte();
         }
      };

      template<>
      struct action_one_a< grammar_one_b > : hierarchical_change_action< grammar_one_b, action_one_b >
      {};

      template<>
      struct action_one_a< grammar_inner >
      {
         template< typename Input >
         static void apply( const Input& in, state_one& state )
         {
            state.byte_in_line_a = in.input().byte();
         }
      };

      void unit_test()
      {
         state_one state{ -1, -1 };
         bool parse_result = parse< grammar_one_a, action_one_a, hierarchical_control >( memory_input( "aa", __FUNCTION__ ), state );
         TAO_PEGTL_TEST_ASSERT( parse_result );
         TAO_PEGTL_TEST_ASSERT( state.byte_in_line_a == 1 );
         TAO_PEGTL_TEST_ASSERT( state.byte_in_line_b == 2 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE
}  // namespace tao

#include "main.hpp"
