// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <vector>

#include "test.hpp"
#include "test_inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   std::vector< std::pair< std::string, std::string > > applied;

   namespace test1
   {
      struct fiz
         : seq< at< one< 'a' > >, two< 'a' > >
      {};

      struct foo
         : sor< fiz, one< 'b' > >
      {};

      struct bar
         : until< eof, foo >
      {};

      void test_result()
      {
         TAO_PEGTL_TEST_ASSERT( applied.size() == 10 );

         TAO_PEGTL_TEST_ASSERT( applied[ 0 ].first == demangle< one< 'b' > >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 1 ].first == demangle< foo >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 2 ].first == demangle< at< one< 'a' > > >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 3 ].first == demangle< two< 'a' > >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 4 ].first == demangle< fiz >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 5 ].first == demangle< foo >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 6 ].first == demangle< one< 'b' > >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 7 ].first == demangle< foo >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 8 ].first == demangle< eof >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 9 ].first == demangle< bar >() );

         TAO_PEGTL_TEST_ASSERT( applied[ 0 ].second == "b" );
         TAO_PEGTL_TEST_ASSERT( applied[ 1 ].second == "b" );
         TAO_PEGTL_TEST_ASSERT( applied[ 2 ].second.empty() );
         TAO_PEGTL_TEST_ASSERT( applied[ 3 ].second == "aa" );
         TAO_PEGTL_TEST_ASSERT( applied[ 4 ].second == "aa" );
         TAO_PEGTL_TEST_ASSERT( applied[ 5 ].second == "aa" );
         TAO_PEGTL_TEST_ASSERT( applied[ 6 ].second == "b" );
         TAO_PEGTL_TEST_ASSERT( applied[ 7 ].second == "b" );
         TAO_PEGTL_TEST_ASSERT( applied[ 8 ].second.empty() );
         TAO_PEGTL_TEST_ASSERT( applied[ 9 ].second == "baab" );
      }

   }  // namespace test1

   template< typename Rule >
   struct test_action
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in )
      {
         applied.emplace_back( demangle< Rule >(), in.string() );
      }
   };

   void unit_test()
   {
      const auto result = parse< disable< test1::bar >, test_action >( test::text_input< ascii::lf >( "baab" ) );
      TAO_PEGTL_TEST_ASSERT( result );
      TAO_PEGTL_TEST_ASSERT( applied.size() == 1 );

      TAO_PEGTL_TEST_ASSERT( applied[ 0 ].first == demangle< disable< test1::bar > >() );
      TAO_PEGTL_TEST_ASSERT( applied[ 0 ].second == "baab" );

      applied.clear();

      parse< at< action< test_action, test1::bar > > >( test::text_input< ascii::lf >( "baab" ) );

      TAO_PEGTL_TEST_ASSERT( applied.empty() );

      applied.clear();

      parse< test1::bar, test_action >( test::text_input< ascii::lf >( "baab" ) );

      test1::test_result();

      applied.clear();

      parse< action< test_action, test1::bar > >( test::text_input< ascii::lf >( "baab" ) );

      test1::test_result();

      applied.clear();

      parse< disable< enable< action< test_action, test1::bar > > > >( test::text_input< ascii::lf >( "baab" ) );

      test1::test_result();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
