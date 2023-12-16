// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_UTILITY_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_integer_fraction( const std::size_t n, const std::size_t d ) noexcept
   {
      return ( n / d ) * d == n;
   }

   template< typename T >
   [[nodiscard]] constexpr bool is_ascii_value( const T t ) noexcept
   {
      if constexpr( std::is_unsigned_v< T > ) {
         return t <= 127;
      }
      else if constexpr( sizeof( T ) == 1 ) {
         return 0 <= t;
      }
      else {
         return ( 0 <= t ) && ( t <= 127 );
      }
   }

   [[nodiscard]] constexpr bool is_utf16_surrogate( const char32_t t ) noexcept
   {
      return ( 0xd800 <= t ) && ( t <= 0xdfff );
   }

   [[nodiscard]] constexpr bool is_utf16_high_surrogate( const char32_t t ) noexcept
   {
      return ( 0xd800 <= t ) && ( t <= 0xdbff );
   }

   [[nodiscard]] constexpr bool is_utf16_low_surrogate( const char32_t t ) noexcept
   {
      return ( 0xdc00 <= t ) && ( t <= 0xdfff );
   }

   [[nodiscard]] constexpr char32_t utf16_surrogate_compose( const char16_t high, const char16_t low ) noexcept
   {
      return ( ( char32_t( high & 0x03ff ) << 10 ) | char32_t( low & 0x03ff ) ) + 0x10000;
   }

   [[nodiscard]] bool is_utf32_codepoint( const char32_t t ) noexcept
   {
      return ( t <= 0x10ffff ) && ( !is_utf16_surrogate( t ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif