// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/internal/ascii_utility.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( char( 0 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( char( 42 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( char( 127 ) ) );

      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( test::choose_char( -1, 128 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( test::choose_char( -42, 200 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( test::choose_char( -127, 255 ) ) );

      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int8_t( 0 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int8_t( 42 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int8_t( 127 ) ) );

      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int8_t( -1 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int8_t( 128 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int8_t( 200 ) ) );

      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint8_t( 0 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint8_t( 42 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint8_t( 127 ) ) );

      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint8_t( -1 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint8_t( 128 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint8_t( 200 ) ) );

      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int32_t( 0 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int32_t( 42 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::int32_t( 127 ) ) );

      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( -1000 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( -1 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( 128 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( 200 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( 200000 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::int32_t( 2000000000 ) ) );

      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint32_t( 0 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint32_t( 42 ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::is_seven_value( std::uint32_t( 127 ) ) );

      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( -1000 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( -1 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( 128 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( 200 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( 200000 ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::is_seven_value( std::uint32_t( 2000000000 ) ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_char_equal< 'a' >( 'a' ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_char_equal< 'D' >( 'D' ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'a' >( 'b' ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'a' >( 'A' ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'A' >( 'a' ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_char_equal< 'a' >( int( 'a' ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'a' >( int( 'A' ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'a' >( int( 'a' ) + 256 ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_char_equal< 'a' >( int( 'A' ) + 256 ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_ichar_equal< 'a' >( 'a' ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_ichar_equal< 'a' >( 'b' ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_ichar_equal< 'a' >( 'A' ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_ichar_equal< 'D' >( 'd' ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_ichar_equal< 'a' >( int( 'a' ) ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_ichar_equal< 'a' >( int( 'A' ) ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_ichar_equal< 'a' >( int( 'a' ) + 256 ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_ichar_equal< 'a' >( int( 'A' ) + 256 ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_string_equal<>( "" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_string_equal< 'a' >( "a" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'a' >( "b" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'a' >( "A" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'M' >( "m" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_string_equal< 'a', 'B', 'c' >( "aBc" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'a', 'B', 'c' >( "aBC" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'a', 'B', 'c' >( "abc" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_string_equal< 'a', 'B', 'c' >( "abC" ) );

      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal<>( "" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a' >( "a" ) );
      TAO_PEGTL_TEST_ASSERT( !internal::ascii_istring_equal< 'a' >( "b" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a' >( "A" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'M' >( "m" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a', 'B', 'c' >( "aBc" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a', 'B', 'c' >( "abC" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a', 'B', 'c' >( "ABC" ) );
      TAO_PEGTL_TEST_ASSERT( internal::ascii_istring_equal< 'a', 'B', 'c' >( "abc" ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
