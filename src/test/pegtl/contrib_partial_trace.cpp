// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/trace.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct inner : seq< one< 'a' >, sor< one< 'b' >, one< 'c' >, inner > > {};
   struct outer : seq< one< 'x' >, inner, one< 'y' > > {};

   // how to run a tracer on a *part* of the grammar:
   template< typename > struct partial_action {};
   template<> struct partial_action< inner > : trace_standard {};
   // clang-format on

   void unit_test()
   {
      memory_input in( "xaacy", "trace test please ignore" );
      const auto result = parse< outer, partial_action >( in );
      TAO_PEGTL_TEST_ASSERT( result );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
