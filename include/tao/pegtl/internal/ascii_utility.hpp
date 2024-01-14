// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_ASCII_UTILITY_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
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

   template< char C >
   inline constexpr bool is_ascii_alpha = ( ( 'a' <= C ) && ( C <= 'z' ) ) || ( ( 'A' <= C ) && ( C <= 'Z' ) );

   template< char C, typename D >
   [[nodiscard]] constexpr bool ascii_char_equal( const D d ) noexcept
   {
      return d == static_cast< D >( C );
   }

   template< char... Cs >
   [[nodiscard]] constexpr bool ascii_string_equal( const char* r ) noexcept
   {
      return ( ascii_char_equal< Cs >( *r++ ) && ... );
   }

   // [[nodiscard]] inline bool ascii_string_equal( const void* s, const std::initializer_list< char >& l ) noexcept
   // {
   //    return std::memcmp( s, &*l.begin(), l.size() ) == 0;
   // }

   template< char C, typename D >
   [[nodiscard]] constexpr bool ascii_ichar_equal( const D d ) noexcept
   {
      if constexpr( is_ascii_alpha< C > ) {
         return ( C | 0x20 ) == ( d | 0x20 );
      }
      else {
         return d == static_cast< D >( C );
      }
   }

   template< char... Cs >
   [[nodiscard]] constexpr bool ascii_istring_equal( const char* r ) noexcept
   {
      return ( ascii_ichar_equal< Cs >( *r++ ) && ... );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif

