// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_DETAILS_HPP
#define TAO_PEGTL_INTERNAL_UTF8_DETAILS_HPP

#include <cstddef>

#include "../config.hpp"

#include "utf16_details.hpp"
#include "utf8_constants.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_utf8_byte_1_of_1( const char32_t t ) noexcept
   {
      return ( t & 0x80 ) == 0;
   }

   [[nodiscard]] constexpr bool is_utf8_byte_1_of_2( const char32_t t ) noexcept
   {
      return ( t & 0xe0 ) == 0xc0;
   }

   [[nodiscard]] constexpr bool is_utf8_byte_1_of_3( const char32_t t ) noexcept
   {
      return ( t & 0xf0 ) == 0xe0;
   }

   [[nodiscard]] constexpr bool is_utf8_byte_1_of_4( const char32_t t ) noexcept
   {
      return ( t & 0xf8 ) == 0xf0;
   }

   [[nodiscard]] constexpr bool is_utf8_continuation( const char32_t t ) noexcept
   {
      return ( t & 0xc0 ) == 0x80;
   }

   [[nodiscard]] constexpr bool has_utf8_length_1( const char32_t t ) noexcept
   {
      return t <= utf8_max_length_1;
   }

   [[nodiscard]] constexpr bool has_utf8_length_2( const char32_t t ) noexcept
   {
      return ( utf8_min_length_2 <= t ) && ( t <= utf8_max_length_2 );
   }

   [[nodiscard]] constexpr bool has_utf8_length_3( const char32_t t ) noexcept
   {
      return ( utf8_min_length_3 <= t ) && ( t <= utf8_max_length_3 ) && ( !is_utf16_surrogate( t ) );
   }

   [[nodiscard]] constexpr bool has_utf8_length_4( const char32_t t ) noexcept
   {
      return ( utf8_min_length_4 <= t ) && ( t <= utf8_max_length_4 );
   }

   [[nodiscard]] constexpr char utf8_char_1_of_2( const char32_t t ) noexcept
   {
      return char( ( ( t >> 6 ) & 0x1f ) | 0xc0 );
   }

   [[nodiscard]] constexpr char utf8_char_2_of_2( const char32_t t ) noexcept
   {
      return char( ( t & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char utf8_char_1_of_3( const char32_t t ) noexcept
   {
      return char( ( ( t >> 12 ) & 0x0f ) | 0xe0 );
   }

   [[nodiscard]] constexpr char utf8_char_2_of_3( const char32_t t ) noexcept
   {
      return char( ( ( t >> 6 ) & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char utf8_char_3_of_3( const char32_t t ) noexcept
   {
      return char( ( t & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char utf8_char_1_of_4( const char32_t t ) noexcept
   {
      return char( ( ( t >> 18 ) & 0x07 ) | 0xf0 );
   }

   [[nodiscard]] constexpr char utf8_char_2_of_4( const char32_t t ) noexcept
   {
      return char( ( ( t >> 12 ) & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char utf8_char_3_of_4( const char32_t t ) noexcept
   {
      return char( ( ( t >> 6 ) & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char utf8_char_4_of_4( const char32_t t ) noexcept
   {
      return char( ( t & 0x3f ) | 0x80 );
   }

   [[nodiscard]] constexpr char32_t utf8_compose( const char32_t c0, const char32_t c1 ) noexcept
   {
      return ( ( c0 & 0x1f ) << 6 ) | ( c1 & 0x3f );
   }

   [[nodiscard]] constexpr char32_t utf8_compose( const char32_t c0, const char32_t c1, const char32_t c2 ) noexcept
   {
      return ( ( c0 & 0x0f ) << 12 ) | ( ( c1 & 0x3f ) << 6 ) | ( c2 & 0x3f );
   }

   [[nodiscard]] constexpr char32_t utf8_compose( const char32_t c0, const char32_t c1, const char32_t c2, const char32_t c3 ) noexcept
   {
      return ( ( c0 & 0x07 ) << 18 ) | ( ( c1 & 0x3f ) << 12 ) | ( ( c2 & 0x3f ) << 6 ) | ( c3 & 0x3f );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
