// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/extra/dispatch.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct action1
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& st )
      {
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         st += in.peek_char();
      }
   };

   template< typename Rule >
   struct action2
   {
      static void apply0( std::string& st )
      {
         st += '2';
      }
   };

   struct grammar
      : seq< one< 'a' >, one< 'b' >, one< 'c' >, one< 'd' >, eof >
   {};

   struct unmatched_grammar
      : seq< one< 'x' >, eof >
   {};

   using clause_a_c = clause1< action1, one< 'a' >, one< 'c' > >;
   using clause_b_d = clause2< action2, one< 'b' >, one< 'd' > >;

   void test_success()
   {
      std::string st;
      view_input<> in( "abcd" );
      const auto b = dispatch< clause_a_c, clause_b_d >::parse< grammar >( in, st );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( st == "a2c2" );
   }

   void test_no_clause_match()
   {
      std::string st;
      view_input<> in( "x" );
      const auto b = dispatch< clause_a_c, clause_b_d >::parse< unmatched_grammar >( in, st );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( st.empty() );
   }

   void test_failure()
   {
      std::string st;
      view_input<> in( "abx" );
      const auto b = dispatch< clause_a_c, clause_b_d >::parse< grammar >( in, st );
      TAO_PEGTL_TEST_ASSERT( !b );
      TAO_PEGTL_TEST_ASSERT( st == "a2" );
   }

   void unit_test()
   {
      test_success();
      test_no_clause_match();
      test_failure();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
