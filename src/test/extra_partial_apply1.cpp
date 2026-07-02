// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <vector>

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/extra/partial_apply1.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace testcase
   {
      std::vector< std::pair< std::string, std::string > > applied;

      struct first
         : seq< one< 'a' > >
      {};

      struct second
         : seq< one< 'b' > >
      {};

      struct third
         : seq< one< 'c' > >
      {};

      struct grammar
         : seq< one< 'x' >, partial_apply1< first, second, third >, eof >
      {};

      template< typename Rule >
      struct action
         : nothing< Rule >
      {};

      struct record_action
      {
         template< typename Rule, typename ActionInput >
         static void apply( const ActionInput& in )
         {
            applied.emplace_back( demangle< Rule >(), in.string() );
         }
      };

      template<>
      struct action< one< 'a' > >
         : record_action
      {};

      template<>
      struct action< one< 'b' > >
         : record_action
      {};

      template<>
      struct action< one< 'c' > >
         : record_action
      {};

      template<>
      struct action< first >
         : record_action
      {};

      template<>
      struct action< second >
         : record_action
      {};

      template<>
      struct action< third >
         : record_action
      {};

      template< typename Rule >
      void verify_action( const std::string& data, const std::string& applied_data )
      {
         applied.clear();

         TAO_PEGTL_TEST_ASSERT( parse< grammar, action >( text_view_input< scan::lf >( data ) ) );
         TAO_PEGTL_TEST_ASSERT( applied.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( applied[ 0 ].first == demangle< Rule >() );
         TAO_PEGTL_TEST_ASSERT( applied[ 0 ].second == applied_data );
      }

   }  // namespace testcase

   void unit_test()
   {
      verify_meta< partial_apply1< alpha, digit >, internal::partial_apply1< alpha, digit >, alpha, digit >();
      verify_meta< partial_apply1< alpha, digit, eof >, internal::partial_apply1< alpha, digit, eof >, alpha, digit, eof >();

      verify_analyze< partial_apply1< any, eof > >( __LINE__, __FILE__, true, false );
      verify_analyze< partial_apply1< any, success > >( __LINE__, __FILE__, true, false );
      verify_analyze< partial_apply1< success, any > >( __LINE__, __FILE__, false, false );
      verify_analyze< partial_apply1< eof, any > >( __LINE__, __FILE__, false, false );

      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::success );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::success );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "b", result_type::local_failure );
      verify_rule< partial_apply1< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ba", result_type::local_failure );

      testcase::verify_action< testcase::first >( "xa", "a" );
      testcase::verify_action< testcase::second >( "xab", "ab" );
      testcase::verify_action< testcase::third >( "xabc", "abc" );

      testcase::applied.clear();
      TAO_PEGTL_TEST_ASSERT( !parse< testcase::grammar, testcase::action >( text_view_input( "x" ) ) );
      TAO_PEGTL_TEST_ASSERT( testcase::applied.empty() );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
