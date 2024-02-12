// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF_ONE_HPP
#define TAO_PEGTL_INTERNAL_UTF_ONE_HPP

#include "../config.hpp"

#include "ascii_string.hpp"
#include "one.hpp"
#include "peek_utf8.hpp"
#include "unicode_to_utf8_char.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, char32_t... Cs >
   struct utf_one
   {
      using type = one< Peek, Cs... >;
   };

   template< char32_t C >
   struct utf_one< peek_utf8, C >
   {
      using type = unicode_to_utf8_char_t< ascii_string, C >;
   };

   template< typename Peek, char32_t... Cs >
   using utf_one_t = typename utf_one< Peek, Cs... >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
