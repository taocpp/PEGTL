// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_ASCII_UTILITY_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char C >
   inline constexpr bool ascii_is_alpha = ( ( 'a' <= C ) && ( C <= 'z' ) ) || ( ( 'A' <= C ) && ( C <= 'Z' ) );

   template< char C, typename D >
   [[nodiscard]] constexpr bool ascii_ichar_equal( const D c ) noexcept
   {
      if constexpr( ascii_is_alpha< C > ) {
         return ( C | 0x20 ) == ( c | 0x20 );
      }
      else {
         return c == C;
      }
      static_assert( std::is_same_v< D, char > );
   }

   template< char... Cs >
   [[nodiscard]] constexpr bool ascii_istring_equal( const char* r ) noexcept
   {
      return ( ascii_ichar_equal< Cs >( *r++ ) && ... );
   }

   [[nodiscard]] inline bool ascii_string_equal( const void* s, const std::initializer_list< char >& l ) noexcept
   {
      return std::memcmp( s, &*l.begin(), l.size() ) == 0;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif

