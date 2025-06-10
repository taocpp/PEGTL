// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <vector>

#include "test.hpp"

#include <tao/pegtl/control/rewind_state_control.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct dummy_state
   {};

   std::vector< std::pair< char, std::size_t > > test_trace;

   struct test_state
   {
      template< typename Rule >
      static constexpr bool enable = false;

      template< typename Rule, typename Input >
      void prep_rewind( const Input& in, const dummy_state& /*unused*/ )
      {
         test_trace.emplace_back( 'G', in.direct_count() );
      }

      template< typename Rule, typename Input >
      void will_rewind( const Input& in, const dummy_state& /*unused*/ )
      {
         test_trace.emplace_back( 'R', in.direct_count() );
      }

      template< typename Rule, typename Input >
      void wont_rewind( const Input& in, const dummy_state& /*unused*/ )
      {
         test_trace.emplace_back( 'C', in.direct_count() );
      }
   };

   struct test_grammar
      : sor< seq< one< 'a' >, one< 'b' > >, one< 'a' > >
   {};

   void test1()
   {
      dummy_state ds;
      test_state ts;
      test_trace.clear();
      text_view_input<> in( "a" );
      const bool result = parse< test_grammar, nothing, rewind_state_control< normal >::type >( in, ds, ts );
      TAO_PEGTL_TEST_ASSERT( result );
      TAO_PEGTL_TEST_ASSERT( test_trace.size() == 2 );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 0 ].first == 'G' );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 0 ].second == 0 );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 1 ].first == 'R' );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 1 ].second == 1 );
   }

   void test2()
   {
      dummy_state ds;
      test_state ts;
      test_trace.clear();
      text_view_input<> in( "ab" );
      const bool result = parse< test_grammar, nothing, rewind_state_control< normal >::type >( in, ds, ts );
      TAO_PEGTL_TEST_ASSERT( result );
      TAO_PEGTL_TEST_ASSERT( test_trace.size() == 2 );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 0 ].first == 'G' );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 0 ].second == 0 );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 1 ].first == 'C' );
      TAO_PEGTL_TEST_ASSERT( test_trace[ 1 ].second == 2 );
   }

   void unit_test()
   {
      test1();
      test2();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
