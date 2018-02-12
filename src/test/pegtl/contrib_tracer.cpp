// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/tracer.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      using GRAMMAR = sor< failure, one< 'a' > >;

      template< typename Rule >
      struct tracer_action
         : nothing< Rule >
      {
      };

      unsigned a0 = 0;
      unsigned a = 0;

      template<>
      struct tracer_action< one< 'a' > >
      {
         template< typename... Ts >
         static void apply0( Ts&&... /*unused*/ )
         {
            ++a0;
         }
      };

      template<>
      struct tracer_action< GRAMMAR >
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
            memory_input<> in( "ab", "trace test please ignore" );
            const auto result = parse< GRAMMAR, nothing, tracer >( in );
            TAO_PEGTL_TEST_ASSERT( result );
            TAO_PEGTL_TEST_ASSERT( a0 == 0 );
            TAO_PEGTL_TEST_ASSERT( a == 0 );
         }
         {
            memory_input<> in( "ab", "trace test please ignore" );
            const auto result = parse< GRAMMAR, tracer_action, tracer >( in );
            TAO_PEGTL_TEST_ASSERT( result );
            TAO_PEGTL_TEST_ASSERT( a0 == 1 );
            TAO_PEGTL_TEST_ASSERT( a == 1 );
         }
         {
            trace_state ts;
            memory_input<> in( "ab", "trace test please ignore" );
            const auto result = parse< GRAMMAR, nothing, tracer >( in, ts );
            TAO_PEGTL_TEST_ASSERT( result );
            TAO_PEGTL_TEST_ASSERT( a0 == 1 );
            TAO_PEGTL_TEST_ASSERT( a == 1 );
         }
         {
            trace_state ts;
            memory_input<> in( "ab", "trace test please ignore" );
            const auto result = parse< GRAMMAR, tracer_action, tracer >( in, ts );
            TAO_PEGTL_TEST_ASSERT( result );
            TAO_PEGTL_TEST_ASSERT( a0 == 2 );
            TAO_PEGTL_TEST_ASSERT( a == 2 );
         }
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
