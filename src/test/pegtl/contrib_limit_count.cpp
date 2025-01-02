// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/contrib/limit_count.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_rule
      : plus< alpha >
   {};

   struct test_grammar
      : seq< test_rule, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< test_rule >
      : limit_count< 5 >
   {};

   void unit_test()
   {
      using test_input = internal::input_with_fakes< internal::input_with_funcs< internal::input_with_start< internal::view_input< char > > > >;

      test_input i1( "aaa" );
      const auto r1 = parse< test_grammar >( i1 );
      TAO_PEGTL_TEST_ASSERT( r1 );

      test_input i2( "aaaaaaaaaaa" );
      const auto r2 = parse< test_grammar >( i2 );
      TAO_PEGTL_TEST_ASSERT( r2 );

      test_input i3( "aaa" );
      const auto r3 = parse< test_grammar, test_action >( i3 );
      TAO_PEGTL_TEST_ASSERT( r3 );

      test_input i4( "000" );
      const auto r4 = parse< test_grammar, test_action >( i4 );
      TAO_PEGTL_TEST_ASSERT( !r4 );

      test_input i5( "0aaaaaaaaaaa" );
      const auto r5 = parse< test_grammar, test_action >( i5 );
      TAO_PEGTL_TEST_ASSERT( !r5 );

#if defined( __cpp_exceptions )
      test_input i6( "aaaaaaaaaaa" );
      TAO_PEGTL_TEST_THROWS( parse< test_grammar, test_action >( i6 ) );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
