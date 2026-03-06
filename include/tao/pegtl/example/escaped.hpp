// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_ESCAPED_HPP
#define TAO_PEGTL_EXAMPLE_ESCAPED_HPP

#include <cstdlib>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "../unicode/utf8.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct c_escaped_char
      : one< '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '0' >
   {};

   struct json_escaped_char
      : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' >
   {};

   struct hex_char_xdigits
      : rep< 2, xdigit >
   {};

   struct hex_escaped_char
      : seq< one< 'x' >, hex_char_xdigits >
   {};

   template< std::size_t N >
   struct rep_unicode_xdigits
      : rep< N, xdigit >
   {};

   struct short_escaped_unicode
      : seq< one< 'u' >, rep_unicode_xdigits< 4 > >
   {};

   struct long_escaped_unicode
      : seq< one< 'U' >, rep_unicode_xdigits< 8 > >
   {};

   struct json_unicode_xdigits
      : rep< 4, xdigit >
   {};

   struct json_escaped_unicode
      : list< seq< one< 'u' >, json_unicode_xdigits >, one< '\\' > >
   {};

   struct json_escaped
      : sor< json_escaped_char, json_escaped_unicode >
   {};

   struct json_character
      : if_then_else< one< '\\' >, json_escaped, utf8::range< 0x20, 0x10ffff > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
