// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/extra/internal/unhex_utility.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( internal::unhex_char< int >( '0' ) == 0 );
   static_assert( internal::unhex_char< int >( '9' ) == 9 );
   static_assert( internal::unhex_char< int >( 'a' ) == 10 );
   static_assert( internal::unhex_char< int >( 'b' ) == 11 );
   static_assert( internal::unhex_char< int >( 'c' ) == 12 );
   static_assert( internal::unhex_char< int >( 'd' ) == 13 );
   static_assert( internal::unhex_char< int >( 'e' ) == 14 );
   static_assert( internal::unhex_char< int >( 'f' ) == 15 );
   static_assert( internal::unhex_char< int >( 'A' ) == 10 );
   static_assert( internal::unhex_char< int >( 'B' ) == 11 );
   static_assert( internal::unhex_char< int >( 'C' ) == 12 );
   static_assert( internal::unhex_char< int >( 'D' ) == 13 );
   static_assert( internal::unhex_char< int >( 'E' ) == 14 );
   static_assert( internal::unhex_char< int >( 'F' ) == 15 );

   static_assert( internal::unhex_char< unsigned >( '0' ) == 0 );
   static_assert( internal::unhex_char< unsigned >( '9' ) == 9 );
   static_assert( internal::unhex_char< unsigned >( 'a' ) == 10 );
   static_assert( internal::unhex_char< unsigned >( 'f' ) == 15 );
   static_assert( internal::unhex_char< unsigned >( 'A' ) == 10 );
   static_assert( internal::unhex_char< unsigned >( 'F' ) == 15 );

   template< std::size_t N >
   void test_unhex( const char ( &s )[ N ], const int r )
   {
      TAO_PEGTL_TEST_ASSERT( internal::unhex_string< N - 1, int >( s ) == r );
      TAO_PEGTL_TEST_ASSERT( internal::unhex_string< N - 1, unsigned >( s ) == unsigned( r ) );
   }

   void unit_test()
   {
      test_unhex( "0", 0 );
      test_unhex( "a0", 0xa0 );
      test_unhex( "1Fb93", 0x1fb93 );
      test_unhex( "ffffff", 0xffffff );
      test_unhex( "abcdef", 0xabcdef );
      test_unhex( "ABCDEF", 0xabcdef );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
