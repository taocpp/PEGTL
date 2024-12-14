// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/unicode.hpp>
#include <tao/pegtl/utf16.hpp>
#include <tao/pegtl/utf32.hpp>

#include <tao/pegtl/int16.hpp>
#include <tao/pegtl/int32.hpp>
#include <tao/pegtl/int64.hpp>
#include <tao/pegtl/int8.hpp>

#include <tao/pegtl/uint16.hpp>
#include <tao/pegtl/uint32.hpp>
#include <tao/pegtl/uint64.hpp>
#include <tao/pegtl/uint8.hpp>

#include <tao/pegtl/enums.hpp>
#include <tao/pegtl/member.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct foo
   {
      int bar;
   };

   // Some compilers simultaneously
   // (a) optimise away every invocation of trivial constexpr functions even with -O0 -fno-inline, but
   // (b) also emit the functions as code, even though they are impossible to call without contortions.
   // For now using std::function bypasses the optimiser so that the functions are called at run-time.

   template< typename R >
   void constexpr_to_runtime_hack( R ( &f )(), const std::decay_t< R > r )  // TODO: Use std::type_identity_t with C++20.
   {
      std::function< R() > u( f );
      TAO_PEGTL_TEST_ASSERT( u() == r );
   }

   template< typename R, typename T >
   void constexpr_to_runtime_hack( R ( &f )( const T ), const std::decay_t< T > t, const std::decay_t< R > r )  // TODO: Use std::type_identity_t with C++20.
   {
      std::function< R( const T ) > u( f );
      TAO_PEGTL_TEST_ASSERT( u( t ) == r );
   }

   void unit_test()
   {
      using in8 = internal::view_input< std::uint8_t >;
      using in16 = internal::view_input< std::uint16_t >;
      using in32 = internal::view_input< std::uint32_t >;
      using in64 = internal::view_input< std::uint64_t >;
      using ins = internal::view_input< std::string >;

      static_assert( internal::peek_data::bulk< ins >() == true );
      static_assert( internal::peek_data::size< ins >() == 1 );

      static_assert( internal::peek_char::bulk< in8 >() == true );
      static_assert( internal::peek_char::size< in8 >() == 1 );

      static_assert( internal::peek_seven::bulk< in8 >() == false );
      static_assert( internal::peek_seven::size< in8 >() == 1 );

      static_assert( internal::peek_utf8::bulk< in8 >() == false );
      static_assert( internal::peek_utf8::size< in8 >() == 0 );

      static_assert( internal::peek_utf16_be::bulk< in8 >() == false );
      static_assert( internal::peek_utf16_be::size< in8 >() == 0 );

      static_assert( internal::peek_utf16_le::bulk< in16 >() == false );
      static_assert( internal::peek_utf16_le::size< in16 >() == 0 );

      static_assert( internal::peek_utf32_be::bulk< in8 >() == false );
      static_assert( internal::peek_utf32_be::size< in8 >() == 4 );

      static_assert( internal::peek_utf32_le::bulk< in32 >() == false );
      static_assert( internal::peek_utf32_le::size< in32 >() == 1 );

      static_assert( internal::peek_int8::bulk< in8 >() == true );
      static_assert( internal::peek_int8::size< in8 >() == 1 );

      static_assert( internal::peek_int16_be::bulk< in8 >() == true );
      static_assert( internal::peek_int16_be::size< in8 >() == 2 );

      static_assert( internal::peek_int16_le::bulk< in16 >() == true );
      static_assert( internal::peek_int16_le::size< in16 >() == 1 );

      static_assert( internal::peek_int32_be::bulk< in8 >() == true );
      static_assert( internal::peek_int32_be::size< in8 >() == 4 );

      static_assert( internal::peek_int32_le::bulk< in32 >() == true );
      static_assert( internal::peek_int32_le::size< in32 >() == 1 );

      static_assert( internal::peek_int64_be::bulk< in8 >() == true );
      static_assert( internal::peek_int64_be::size< in8 >() == 8 );

      static_assert( internal::peek_int64_le::bulk< in64 >() == true );
      static_assert( internal::peek_int64_le::size< in64 >() == 1 );

      static_assert( internal::peek_uint8::bulk< in8 >() == true );
      static_assert( internal::peek_uint8::size< in8 >() == 1 );

      static_assert( internal::peek_uint16_be::bulk< in8 >() == true );
      static_assert( internal::peek_uint16_be::size< in8 >() == 2 );

      static_assert( internal::peek_uint16_le::bulk< in16 >() == true );
      static_assert( internal::peek_uint16_le::size< in16 >() == 1 );

      static_assert( internal::peek_uint32_be::bulk< in8 >() == true );
      static_assert( internal::peek_uint32_be::size< in8 >() == 4 );

      static_assert( internal::peek_uint32_le::bulk< in32 >() == true );
      static_assert( internal::peek_uint32_le::size< in32 >() == 1 );

      static_assert( internal::peek_uint64_be::bulk< in8 >() == true );
      static_assert( internal::peek_uint64_be::size< in8 >() == 8 );

      static_assert( internal::peek_uint64_le::bulk< in64 >() == true );
      static_assert( internal::peek_uint64_le::size< in64 >() == 1 );

      static_assert( internal::peek_mask_int8< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_int8< 43 >::size< in8 >() == 1 );

      static_assert( internal::peek_mask_int16_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_int16_be< 43 >::size< in8 >() == 2 );

      static_assert( internal::peek_mask_int16_le< 42 >::bulk< in16 >() == true );
      static_assert( internal::peek_mask_int16_le< 43 >::size< in16 >() == 1 );

      static_assert( internal::peek_mask_int32_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_int32_be< 43 >::size< in8 >() == 4 );

      static_assert( internal::peek_mask_int32_le< 42 >::bulk< in32 >() == true );
      static_assert( internal::peek_mask_int32_le< 43 >::size< in32 >() == 1 );

      static_assert( internal::peek_mask_int64_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_int64_be< 43 >::size< in8 >() == 8 );

      static_assert( internal::peek_mask_int64_le< 42 >::bulk< in64 >() == true );
      static_assert( internal::peek_mask_int64_le< 43 >::size< in64 >() == 1 );

      static_assert( internal::peek_mask_uint8< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_uint8< 43 >::size< in8 >() == 1 );

      static_assert( internal::peek_mask_uint16_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_uint16_be< 43 >::size< in8 >() == 2 );

      static_assert( internal::peek_mask_uint16_le< 42 >::bulk< in16 >() == true );
      static_assert( internal::peek_mask_uint16_le< 43 >::size< in16 >() == 1 );

      static_assert( internal::peek_mask_uint32_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_uint32_be< 43 >::size< in8 >() == 4 );

      static_assert( internal::peek_mask_uint32_le< 42 >::bulk< in32 >() == true );
      static_assert( internal::peek_mask_uint32_le< 43 >::size< in32 >() == 1 );

      static_assert( internal::peek_mask_uint64_be< 42 >::bulk< in8 >() == true );
      static_assert( internal::peek_mask_uint64_be< 43 >::size< in8 >() == 8 );

      static_assert( internal::peek_mask_uint64_le< 42 >::bulk< in64 >() == true );
      static_assert( internal::peek_mask_uint64_le< 43 >::size< in64 >() == 1 );

      static_assert( internal::peek_member< &foo::bar >::bulk< void >() == true );
      static_assert( internal::peek_member< &foo::bar >::size< void >() == 1 );

      static_assert( internal::peek_unicode::bulk< ins >() == false );

      static_assert( internal::has_utf8_length_1( 0x0000 ) == true );
      static_assert( internal::has_utf8_length_1( 0x007f ) == true );
      static_assert( internal::has_utf8_length_1( 0x0080 ) == false );
      static_assert( internal::has_utf8_length_1( 0xffff ) == false );

      static_assert( internal::has_utf8_length_2( 0x007f ) == false );
      static_assert( internal::has_utf8_length_2( 0x0080 ) == true );
      static_assert( internal::has_utf8_length_2( 0x07ff ) == true );
      static_assert( internal::has_utf8_length_2( 0x0800 ) == false );

      static_assert( internal::has_utf8_length_3( 0x007ff ) == false );
      static_assert( internal::has_utf8_length_3( 0x00800 ) == true );
      static_assert( internal::has_utf8_length_3( 0x0ffff ) == true );
      static_assert( internal::has_utf8_length_3( 0x10000 ) == false );

      static_assert( internal::has_utf8_length_3( 0x0d7ff ) == true );
      static_assert( internal::has_utf8_length_3( 0x0d800 ) == false );
      static_assert( internal::has_utf8_length_3( 0x0dfff ) == false );
      static_assert( internal::has_utf8_length_3( 0x0e000 ) == true );

      static_assert( internal::has_utf8_length_4( 0x00ffff ) == false );
      static_assert( internal::has_utf8_length_4( 0x010000 ) == true );
      static_assert( internal::has_utf8_length_4( 0x10ffff ) == true );
      static_assert( internal::has_utf8_length_4( 0x110000 ) == false );

      constexpr_to_runtime_hack( internal::peek_data::bulk< ins >, true );
      constexpr_to_runtime_hack( internal::peek_data::size< ins >, 1 );

      constexpr_to_runtime_hack( internal::peek_char::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_char::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_seven::bulk< in8 >, false );
      constexpr_to_runtime_hack( internal::peek_seven::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_utf8::bulk< in8 >, false );
      constexpr_to_runtime_hack( internal::peek_utf8::size< in8 >, 0 );

      constexpr_to_runtime_hack( internal::peek_utf16_be::bulk< in8 >, false );
      constexpr_to_runtime_hack( internal::peek_utf16_be::size< in8 >, 0 );

      constexpr_to_runtime_hack( internal::peek_utf16_le::bulk< in16 >, false );
      constexpr_to_runtime_hack( internal::peek_utf16_le::size< in16 >, 0 );

      constexpr_to_runtime_hack( internal::peek_utf32_be::bulk< in8 >, false );
      constexpr_to_runtime_hack( internal::peek_utf32_be::size< in8 >, 4 );

      constexpr_to_runtime_hack( internal::peek_utf32_le::bulk< in32 >, false );
      constexpr_to_runtime_hack( internal::peek_utf32_le::size< in32 >, 1 );

      constexpr_to_runtime_hack( internal::peek_int8::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_int8::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_int16_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_int16_be::size< in8 >, 2 );

      constexpr_to_runtime_hack( internal::peek_int16_le::bulk< in16 >, true );
      constexpr_to_runtime_hack( internal::peek_int16_le::size< in16 >, 1 );

      constexpr_to_runtime_hack( internal::peek_int32_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_int32_be::size< in8 >, 4 );

      constexpr_to_runtime_hack( internal::peek_int32_le::bulk< in32 >, true );
      constexpr_to_runtime_hack( internal::peek_int32_le::size< in32 >, 1 );

      constexpr_to_runtime_hack( internal::peek_int64_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_int64_be::size< in8 >, 8 );

      constexpr_to_runtime_hack( internal::peek_int64_le::bulk< in64 >, true );
      constexpr_to_runtime_hack( internal::peek_int64_le::size< in64 >, 1 );

      constexpr_to_runtime_hack( internal::peek_uint8::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_uint8::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_uint16_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_uint16_be::size< in8 >, 2 );

      constexpr_to_runtime_hack( internal::peek_uint16_le::bulk< in16 >, true );
      constexpr_to_runtime_hack( internal::peek_uint16_le::size< in16 >, 1 );

      constexpr_to_runtime_hack( internal::peek_uint32_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_uint32_be::size< in8 >, 4 );

      constexpr_to_runtime_hack( internal::peek_uint32_le::bulk< in32 >, true );
      constexpr_to_runtime_hack( internal::peek_uint32_le::size< in32 >, 1 );

      constexpr_to_runtime_hack( internal::peek_uint64_be::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_uint64_be::size< in8 >, 8 );

      constexpr_to_runtime_hack( internal::peek_uint64_le::bulk< in64 >, true );
      constexpr_to_runtime_hack( internal::peek_uint64_le::size< in64 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_int8< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int8< 43 >::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_int16_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int16_be< 43 >::size< in8 >, 2 );

      constexpr_to_runtime_hack( internal::peek_mask_int16_le< 42 >::bulk< in16 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int16_le< 43 >::size< in16 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_int32_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int32_be< 43 >::size< in8 >, 4 );

      constexpr_to_runtime_hack( internal::peek_mask_int32_le< 42 >::bulk< in32 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int32_le< 43 >::size< in32 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_int64_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int64_be< 43 >::size< in8 >, 8 );

      constexpr_to_runtime_hack( internal::peek_mask_int64_le< 42 >::bulk< in64 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_int64_le< 43 >::size< in64 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_uint8< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint8< 43 >::size< in8 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_uint16_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint16_be< 43 >::size< in8 >, 2 );

      constexpr_to_runtime_hack( internal::peek_mask_uint16_le< 42 >::bulk< in16 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint16_le< 43 >::size< in16 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_uint32_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint32_be< 43 >::size< in8 >, 4 );

      constexpr_to_runtime_hack( internal::peek_mask_uint32_le< 42 >::bulk< in32 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint32_le< 43 >::size< in32 >, 1 );

      constexpr_to_runtime_hack( internal::peek_mask_uint64_be< 42 >::bulk< in8 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint64_be< 43 >::size< in8 >, 8 );

      constexpr_to_runtime_hack( internal::peek_mask_uint64_le< 42 >::bulk< in64 >, true );
      constexpr_to_runtime_hack( internal::peek_mask_uint64_le< 43 >::size< in64 >, 1 );

      constexpr_to_runtime_hack( internal::peek_member< &foo::bar >::bulk< void >, true );
      constexpr_to_runtime_hack( internal::peek_member< &foo::bar >::size< void >, 1 );

      constexpr_to_runtime_hack( internal::peek_unicode::bulk< ins >, false );

      constexpr_to_runtime_hack( internal::has_utf8_length_1, 0x0000, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_1, 0x007f, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_1, 0x0080, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_1, 0xffff, false );

      constexpr_to_runtime_hack( internal::has_utf8_length_2, 0x007f, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_2, 0x0080, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_2, 0x07ff, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_2, 0x0800, false );

      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x007ff, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x00800, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x0ffff, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x10000, false );

      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x0d7ff, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x0d800, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x0dfff, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_3, 0x0e000, true );

      constexpr_to_runtime_hack( internal::has_utf8_length_4, 0x00ffff, false );
      constexpr_to_runtime_hack( internal::has_utf8_length_4, 0x010000, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_4, 0x10ffff, true );
      constexpr_to_runtime_hack( internal::has_utf8_length_4, 0x110000, false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
