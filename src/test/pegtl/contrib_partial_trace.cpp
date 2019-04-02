// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/tracer.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      // clang-format off
      struct inner : seq< one< 'a' >, sor< one< 'b' >, one< 'c' > > > {};
      struct outer : seq< one< 'x' >, inner, one< 'y' > > {};

      // how to run a tracer on a *part* of the grammar:
      template< typename > struct partial_action {};
      template<> struct partial_action< inner > : change_control< tracer > {};
      // clang-format on

      void unit_test()
      {
         memory_input<> in( "xacy", "trace test please ignore" );
         const auto result = parse< outer, partial_action >( in );
         TAO_PEGTL_TEST_ASSERT( result );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
