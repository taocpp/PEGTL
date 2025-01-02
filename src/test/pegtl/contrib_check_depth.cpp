// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/contrib/check_depth.hpp>
#include <tao/pegtl/contrib/input_with_depth.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_recursive
      : seq< alpha, opt< test_recursive > >
   {};

   struct test_grammar
      : seq< test_recursive, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< test_recursive >
      : check_depth< 5 >
   {};

   void unit_test()
   {
      using memory_input_with_depth = input_with_depth< text_view_input< scan::lf > >;

      memory_input_with_depth i1( "aaa" );
      const auto r1 = parse< test_grammar >( i1 );
      TAO_PEGTL_TEST_ASSERT( r1 );

      memory_input_with_depth i2( "aaaaaaaaaaa" );
      const auto r2 = parse< test_grammar >( i2 );
      TAO_PEGTL_TEST_ASSERT( r2 );

      memory_input_with_depth i3( "aaa" );
      const auto r3 = parse< test_grammar, test_action >( i3 );
      TAO_PEGTL_TEST_ASSERT( r3 );

#if defined( __cpp_exceptions )
      memory_input_with_depth i4( "aaaaaaaaaaa" );
      TAO_PEGTL_TEST_THROWS( parse< test_grammar, test_action >( i4 ) );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
