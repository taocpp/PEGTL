// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF32_TO_UTF8_HPP
#define TAO_PEGTL_INTERNAL_UTF32_TO_UTF8_HPP

#include <type_traits>

#include "../config.hpp"

#include "ascii_string.hpp"
#include "utf8_details.hpp"
#include "value_list_concat.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< char... > class String, char32_t C, typename = void >
   struct utf32_to_utf8_impl;

   template< template< char... > class String, char32_t C >
   struct utf32_to_utf8_impl< String, C, std::enable_if_t< has_utf8_length_1( C ) > >
   {
      using type = String< char( C ) >;
   };

   template< template< char... > class String, char32_t C >
   struct utf32_to_utf8_impl< String, C, std::enable_if_t< has_utf8_length_2( C ) > >
   {
      using type = String< utf8_char_1_of_2( C ),
                           utf8_char_2_of_2( C ) >;
   };

   template< template< char... > class String, char32_t C >
   struct utf32_to_utf8_impl< String, C, std::enable_if_t< has_utf8_length_3( C ) > >
   {
      using type = String< utf8_char_1_of_3( C ),
                           utf8_char_2_of_3( C ),
                           utf8_char_3_of_3( C ) >;
   };
   template< template< char... > class String, char32_t C >
   struct utf32_to_utf8_impl< String, C, std::enable_if_t< has_utf8_length_4( C ) > >
   {
      using type = String< utf8_char_1_of_4( C ),
                           utf8_char_2_of_4( C ),
                           utf8_char_3_of_4( C ),
                           utf8_char_4_of_4( C ) >;
   };

   template< template< char... > class String, char32_t... Cs >
   struct utf32_to_utf8
   {
      using type = value_list_concat_t< typename utf32_to_utf8_impl< String, Cs >::type... >;
   };

   template< template< char... > class String, char32_t C >
   struct utf32_to_utf8< String, C >
   {
      using type = typename utf32_to_utf8_impl< String, C >::type;
   };

   template< template< char... > class String >
   struct utf32_to_utf8< String >
   {
      using type = String<>;
   };

   template< char32_t...  Cs >
   using utf32_to_utf8_t = typename utf32_to_utf8< ascii_string, Cs... >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
