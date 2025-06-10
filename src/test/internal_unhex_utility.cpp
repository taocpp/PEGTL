// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/unhex_utility.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( internal::unhex_char_impl< int >( '0' ) == 0 );
   static_assert( internal::unhex_char_impl< int >( '9' ) == 9 );
   static_assert( internal::unhex_char_impl< int >( 'a' ) == 10 );
   static_assert( internal::unhex_char_impl< int >( 'b' ) == 11 );
   static_assert( internal::unhex_char_impl< int >( 'c' ) == 12 );
   static_assert( internal::unhex_char_impl< int >( 'd' ) == 13 );
   static_assert( internal::unhex_char_impl< int >( 'e' ) == 14 );
   static_assert( internal::unhex_char_impl< int >( 'f' ) == 15 );
   static_assert( internal::unhex_char_impl< int >( 'A' ) == 10 );
   static_assert( internal::unhex_char_impl< int >( 'B' ) == 11 );
   static_assert( internal::unhex_char_impl< int >( 'C' ) == 12 );
   static_assert( internal::unhex_char_impl< int >( 'D' ) == 13 );
   static_assert( internal::unhex_char_impl< int >( 'E' ) == 14 );
   static_assert( internal::unhex_char_impl< int >( 'F' ) == 15 );

   static_assert( internal::unhex_char_impl< unsigned >( '0' ) == 0 );
   static_assert( internal::unhex_char_impl< unsigned >( '9' ) == 9 );
   static_assert( internal::unhex_char_impl< unsigned >( 'a' ) == 10 );
   static_assert( internal::unhex_char_impl< unsigned >( 'f' ) == 15 );
   static_assert( internal::unhex_char_impl< unsigned >( 'A' ) == 10 );
   static_assert( internal::unhex_char_impl< unsigned >( 'F' ) == 15 );

   void test_unhex( const std::string& s, const int r )
   {
      TAO_PEGTL_TEST_ASSERT( internal::unhex_string_impl< int >( s.data(), s.data() + s.size() ) == r );
      TAO_PEGTL_TEST_ASSERT( internal::unhex_string_impl< unsigned >( s.data(), s.data() + s.size() ) == unsigned( r ) );
   }

   void unit_test()
   {
      test_unhex( "", 0 );
      test_unhex( "0", 0 );
      test_unhex( "a0", 0xa0 );
      test_unhex( "1Fb93", 0x1fb93 );
      test_unhex( "ffffff", 0xffffff );
      test_unhex( "abcdef", 0xabcdef );
      test_unhex( "ABCDEF", 0xabcdef );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
