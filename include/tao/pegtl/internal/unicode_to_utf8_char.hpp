// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_CHAR_HPP
#define TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_CHAR_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< char... > class String, char32_t Char, typename = void >
   struct unicode_to_utf8_char;

   template< template< char... > class String, char32_t Char >
   struct unicode_to_utf8_char< String, Char, std::enable_if_t< ( Char <= 0x7f ) > >
   {
      using type = String< char( Char ) >;
   };

   template< template< char... > class String, char32_t Char >
   struct unicode_to_utf8_char< String, Char, std::enable_if_t< ( 0x80 <= Char ) && ( Char <= 0x7ff ) > >
   {
      using type = String< char( ( ( Char >> 6 ) & 0x1f ) | 0xc0 ), char( ( Char & 0x3f ) | 0x80 ) >;
   };

   template< template< char... > class String, char32_t Char >
   struct unicode_to_utf8_char< String, Char, std::enable_if_t< ( 0x800 <= Char ) && ( Char <= 0xffff ) > >
   {
      using type = String< char( ( ( Char >> 12 ) & 0x0f ) | 0xe0 ), char( ( ( Char >> 6 ) & 0x3f ) | 0x80 ) , char( ( Char & 0x3f ) | 0x80 ) >;
   };
   template< template< char... > class String, char32_t Char >
   struct unicode_to_utf8_char< String, Char, std::enable_if_t< ( 0x10000 <= Char ) && ( Char <= 0x10ffff ) > >
   {
      using type = String< char( ( ( Char >> 18 ) & 0x07 ) | 0xf0 ), char( ( ( Char >> 12 ) & 0x3f ) | 0x80 ), char( ( ( Char >> 6 ) & 0x3f ) | 0x80 ), char( ( Char & 0x3f ) | 0x80 ) >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
