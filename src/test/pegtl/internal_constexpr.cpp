// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/utf16.hpp>
#include <tao/pegtl/utf32.hpp>

#include <tao/pegtl/int8.hpp>
#include <tao/pegtl/int16.hpp>
#include <tao/pegtl/int32.hpp>
#include <tao/pegtl/int64.hpp>

#include <tao/pegtl/uint8.hpp>
#include <tao/pegtl/uint16.hpp>
#include <tao/pegtl/uint32.hpp>
#include <tao/pegtl/uint64.hpp>

#include <tao/pegtl/enums.hpp>
#include <tao/pegtl/member.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct foo
   {
      int bar;
   };

   void unit_test()
   {
      using in8 = internal::view_input< std::uint8_t >;
      using in16 = internal::view_input< std::uint16_t >;
      using in32 = internal::view_input< std::uint32_t >;
      using in64 = internal::view_input< std::uint64_t >;

      TAO_PEGTL_TEST_ASSERT( internal::peek_char::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_char::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_seven::bulk< in8 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_seven::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_utf8::bulk< in8 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_utf8::size< in8 >() == 0 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_utf16_be::bulk< in8 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_utf16_be::size< in8 >() == 0 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_utf16_le::bulk< in16 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_utf16_le::size< in16 >() == 0 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_utf32_be::bulk< in8 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_utf32_be::size< in8 >() == 4 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_utf32_le::bulk< in32 >() == false );
      TAO_PEGTL_TEST_ASSERT( internal::peek_utf32_le::size< in32 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int8::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int8::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int16_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int16_be::size< in8 >() == 2 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int16_le::bulk< in16 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int16_le::size< in16 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int32_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int32_be::size< in8 >() == 4 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int32_le::bulk< in32 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int32_le::size< in32 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int64_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int64_be::size< in8 >() == 8 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_int64_le::bulk< in64 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_int64_le::size< in64 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint8::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint8::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint16_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint16_be::size< in8 >() == 2 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint16_le::bulk< in16 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint16_le::size< in16 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint32_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint32_be::size< in8 >() == 4 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint32_le::bulk< in32 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint32_le::size< in32 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint64_be::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint64_be::size< in8 >() == 8 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_uint64_le::bulk< in64 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_uint64_le::size< in64 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int8< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int8< 43 >::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int16_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int16_be< 43 >::size< in8 >() == 2 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int16_le< 42 >::bulk< in16 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int16_le< 43 >::size< in16 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int32_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int32_be< 43 >::size< in8 >() == 4 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int32_le< 42 >::bulk< in32 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int32_le< 43 >::size< in32 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int64_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int64_be< 43 >::size< in8 >() == 8 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int64_le< 42 >::bulk< in64 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_int64_le< 43 >::size< in64 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint8< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint8< 43 >::size< in8 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint16_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint16_be< 43 >::size< in8 >() == 2 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint16_le< 42 >::bulk< in16 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint16_le< 43 >::size< in16 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint32_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint32_be< 43 >::size< in8 >() == 4 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint32_le< 42 >::bulk< in32 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint32_le< 43 >::size< in32 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint64_be< 42 >::bulk< in8 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint64_be< 43 >::size< in8 >() == 8 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint64_le< 42 >::bulk< in64 >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_mask_uint64_le< 43 >::size< in64 >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::peek_member< &foo::bar >::bulk< void >() == true );
      TAO_PEGTL_TEST_ASSERT( internal::peek_member< &foo::bar >::size< void >() == 1 );

      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_1( 0x0000 ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_1( 0x007f ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_1( 0x0080 ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_1( 0xffff ) == false );

      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_2( 0x007f ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_2( 0x0080 ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_2( 0x07ff ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_2( 0x0800 ) == false );

      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x007ff ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x00800 ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x0ffff ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x10000 ) == false );

      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x0d7ff ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x0d800 ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x0dfff ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_3( 0x0e000 ) == true );

      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_4( 0x00ffff ) == false );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_4( 0x010000 ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_4( 0x10ffff ) == true );
      TAO_PEGTL_TEST_ASSERT( internal::has_utf8_length_4( 0x110000 ) == false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
