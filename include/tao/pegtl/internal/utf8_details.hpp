// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_DETAILS_HPP
#define TAO_PEGTL_INTERNAL_UTF8_DETAILS_HPP

#include <cstddef>

#include "../config.hpp"

#include "utf16_details.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_utf8_code_unit_1_of_1( const char32_t t ) noexcept
   {
      return ( t & 0x80 ) == 0;
   }

   [[nodiscard]] constexpr bool is_utf8_code_unit_1_of_2( const char32_t t ) noexcept
   {
      return ( t & 0xe0 ) == 0xc0;
   }

   [[nodiscard]] constexpr bool is_utf8_code_unit_1_of_3( const char32_t t ) noexcept
   {
      return ( t & 0xf0 ) == 0xe0;
   }

   [[nodiscard]] constexpr bool is_utf8_code_unit_1_of_4( const char32_t t ) noexcept
   {
      return ( t & 0xf8 ) == 0xf0;
   }

   [[nodiscard]] constexpr bool is_utf8_continuation( const char32_t t ) noexcept
   {
      return ( t & 0xc0 ) == 0x80;
   }

   [[nodiscard]] constexpr bool utf8_needs_1_code_unit( const char32_t t ) noexcept
   {
      return t <= 0x7f;
   }

   [[nodiscard]] constexpr bool utf8_needs_2_code_units( const char32_t t ) noexcept
   {
      return ( 0x80 <= t ) && ( t <= 0x7ff );
   }

   [[nodiscard]] constexpr bool utf8_needs_3_code_units( const char32_t t ) noexcept
   {
      return ( 0x800 <= t ) && ( t <= 0xffff ) && ( !is_utf16_surrogate( t ) );
   }

   [[nodiscard]] constexpr bool utf8_needs_4_code_units( const char32_t t ) noexcept
   {
      return ( 0x10000 <= t ) && ( t <= 0x10ffff );
   }

   [[nodiscard]] constexpr std::size_t utf8_needed_code_units( const char32_t t ) noexcept
   {
      if( utf8_needs_1_code_unit( t ) ) {
         return 1;
      }
      if( utf8_needs_2_code_units( t ) ) {
         return 2;
      }
      if( utf8_needs_3_code_units( t ) ) {
         return 3;
      }
      if( utf8_needs_4_code_units( t ) ) {
         return 4;
      }
      return 0;  // TODO?
   }

   [[nodiscard]] constexpr char32_t utf8_sequence_compose( const char32_t c0, const char32_t c1 ) noexcept
   {
      return ( ( c0 & 0x1f ) << 6 ) | ( c1 & 0x3f );
   }

   [[nodiscard]] constexpr char32_t utf8_sequence_compose( const char32_t c0, const char32_t c1, const char32_t c2 ) noexcept
   {
      return ( ( c0 & 0x0f ) << 12 ) | ( ( c1 & 0x3f ) << 6 ) | ( c2 & 0x3f );
   }

   [[nodiscard]] constexpr char32_t utf8_sequence_compose( const char32_t c0, const char32_t c1, const char32_t c2, const char32_t c3 ) noexcept
   {
      return ( ( c0 & 0x07 ) << 18 ) | ( ( c1 & 0x3f ) << 12 ) | ( ( c2 & 0x3f ) << 6 ) | ( c3 & 0x3f );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
