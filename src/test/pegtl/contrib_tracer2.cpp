// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/trace2.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using GRAMMAR = sor< failure, one< 'a' > >;
   using GRAMMAR2 = seq< one< 'a' >, any, any, any, any, one< 'b' >, eof >;

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
   struct trace_action< GRAMMAR >
   {
      template< typename... Ts >
      static void apply( Ts&&... /*unused*/ )
      {
         ++a;
      }
   };

   void unit_test()
   {
      {
         memory_input in( "ab", "trace test please ignore" );
         const auto result = trace< GRAMMAR >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( a0 == 0 );
         TAO_PEGTL_TEST_ASSERT( a == 0 );
      }
      {
         memory_input in( "ab", "trace test please ignore" );
         const auto result = trace< GRAMMAR, trace_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( a0 == 1 );
         TAO_PEGTL_TEST_ASSERT( a == 1 );
      }
      {
         memory_input in( "a\r\n\t\0b", 6, "trace test please ignore" );
         const auto result = trace< GRAMMAR2 >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( a0 == 1 );
         TAO_PEGTL_TEST_ASSERT( a == 1 );
      }
      {
         memory_input in( "a\r\n\t\0b", 6, "trace test please ignore" );
         const auto result = trace< GRAMMAR2, trace_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( a0 == 2 );
         TAO_PEGTL_TEST_ASSERT( a == 1 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
