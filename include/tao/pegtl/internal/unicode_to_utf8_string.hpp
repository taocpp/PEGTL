// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_STRING_HPP
#define TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_STRING_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< char32_t... Cs >
   struct unicode_list
   {};

   template< typename Done, typename List, typename = void >
   struct unicode_to_utf8_string;

   template< typename Done >
   struct unicode_to_utf8_string< Done, unicode_list<>, void >
   {
      using type = Done;
   };

   template< template< char... > class String, char... Chars, char32_t Todo, char32_t... Todos >
   struct unicode_to_utf8_string< String< Chars... >, unicode_list< Todo, Todos... >, std::enable_if_t< ( Todo <= 0x7f ) > >
   {
      using type = typename unicode_to_utf8_string< String< Chars..., char( Todo ) >, unicode_list< Todos... > >::type;
   };

   template< template< char... > class String, char... Chars, char32_t Todo, char32_t... Todos >
   struct unicode_to_utf8_string< String< Chars... >, unicode_list< Todo, Todos... >, std::enable_if_t< ( 0x80 <= Todo ) && ( Todo <= 0x7ff ) > >
   {
      using type = typename unicode_to_utf8_string< String< Chars..., char( ( ( Todo >> 6 ) & 0x1f ) | 0xc0 ), char( ( Todo & 0x3f ) | 0x80 ) >, unicode_list< Todos... > >::type;
   };

   template< template< char... > class String, char... Chars, char32_t Todo, char32_t... Todos >
   struct unicode_to_utf8_string< String< Chars... >, unicode_list< Todo, Todos... >, std::enable_if_t< ( 0x800 <= Todo ) && ( Todo <= 0xffff ) > >
   {
      using type = typename unicode_to_utf8_string< String< Chars..., char( ( ( Todo >> 12 ) & 0x0f ) | 0xe0 ), char( ( ( Todo >> 6 ) & 0x3f ) | 0x80 ) , char( ( Todo & 0x3f ) | 0x80 ) >, unicode_list< Todos... > >::type;
   };

   template< template< char... > class String, char... Chars, char32_t Todo, char32_t... Todos >
   struct unicode_to_utf8_string< String< Chars... >, unicode_list< Todo, Todos... >, std::enable_if_t< ( 0x10000 <= Todo ) && ( Todo <= 0x10ffff ) > >
   {
      using type = typename unicode_to_utf8_string< String< Chars..., char( ( ( Todo >> 18 ) & 0x07 ) | 0xf0 ), char( ( ( Todo >> 12 ) & 0x3f ) | 0x80 ), char( ( ( Todo >> 6 ) & 0x3f ) | 0x80 ), char( ( Todo & 0x3f ) | 0x80 ) >, unicode_list< Todos... > >::type;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
