// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/debug/trace.hpp>

namespace testcase
{
   using namespace TAO_PEGTL_NAMESPACE;

   using GRAMMAR1 = sor< failure, one< 'a' > >;
   using GRAMMAR2 = seq< one< 'a' >, any, any, any, any, one< 'b' >, eof >;
   using GRAMMAR3 = sor< one< 'a' >, one< 'b' > >;
#if defined( __cpp_exceptions )
   using GRAMMAR4 = try_catch_return_false< sor< one< 'a' >, must< one< 'b' > > > >;
#endif

   template< typename Rule >
   struct trace_action
   {};

   unsigned a0 = 0;
   unsigned a = 0;

   template<>
   struct trace_action< one< 'a' > >
   {
      template< typename... Ts >
      static void apply0( Ts&&... /*unused*/ )
      {
         ++a0;
      }
   };

   template<>
   struct trace_action< GRAMMAR1 >
   {
      template< typename... Ts >
      static void apply( Ts&&... /*unused*/ )
      {
         ++a;
      }
   };

}  // namespace testcase

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      {
         text_view_input< scan::lf > in( "ab" );
         const auto result = standard_trace< testcase::GRAMMAR1 >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( testcase::a0 == 0 );
         TAO_PEGTL_TEST_ASSERT( testcase::a == 0 );
      }
      {
         text_view_input< lazy::lf > in( "ab" );
         const auto result = standard_trace< testcase::GRAMMAR1, testcase::trace_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( testcase::a0 == 1 );
         TAO_PEGTL_TEST_ASSERT( testcase::a == 1 );
      }
      {
         text_view_input< scan::lf > in( "a\r\n\t\0b", 6 );
         const auto result = standard_trace< testcase::GRAMMAR2 >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( testcase::a0 == 1 );
         TAO_PEGTL_TEST_ASSERT( testcase::a == 1 );
      }
      {
         text_view_input< scan::lf > in( "a\r\n\t\0b", 6 );
         const auto result = standard_trace< testcase::GRAMMAR2, testcase::trace_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( testcase::a0 == 2 );
         TAO_PEGTL_TEST_ASSERT( testcase::a == 1 );
      }
      {
         text_view_input< scan::lf > in( "c" );
         const auto result = standard_trace< testcase::GRAMMAR3 >( in );
         TAO_PEGTL_TEST_ASSERT( !result );
      }
#if defined( __cpp_exceptions )
      {
         text_view_input< scan::lf > in( "c" );
         const auto result = standard_trace< testcase::GRAMMAR4 >( in );
         TAO_PEGTL_TEST_ASSERT( !result );
      }
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
