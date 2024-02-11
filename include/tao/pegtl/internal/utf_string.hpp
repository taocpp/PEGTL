// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF_STRING_HPP
#define TAO_PEGTL_INTERNAL_UTF_STRING_HPP

#include "../config.hpp"

#include "ascii_string.hpp"
#include "one.hpp"
#include "peek_unicode.hpp"
#include "peek_utf8.hpp"
#include "seq.hpp"
#include "unicode_string.hpp"
#include "unicode_to_utf8_string.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, char32_t... Cs >
   struct utf_string
      : seq< one< Peek, Cs >... >
   {
      using type = seq< one< Peek, Cs >... >;
   };

   template< char32_t... Cs >
   struct utf_string< peek_utf8, Cs... >
   {
      using type = unicode_to_utf8_string_t< ascii_string, Cs... >;
   };

   template< char32_t... Cs >
   struct utf_string< peek_unicode, Cs... >
   {
      using type = unicode_string< Cs... >;
   };

   template< typename Peek, char32_t... Cs >
   using utf_string_t = typename utf_string< Peek, Cs...>::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
