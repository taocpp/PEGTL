// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_JSON_HH
#define PEGTL_CONTRIB_JSON_HH

#include "../rules.hh"
#include "../ascii.hh"
#include "../utf8.hh"
#include "abnf.hh"

namespace pegtl
{
   namespace json
   {
      // JSON grammar according to RFC 7159 (for UTF-8 encoded JSON only).

      struct ws : one< ' ', '\t', '\n', '\r' > {};

      struct begin_array : pad< one< '[' >, ws > {};
      struct begin_object : pad< one< '{' >, ws > {};
      struct end_array : pad< one< ']' >, ws > {};
      struct end_object : pad< one< '}' >, ws > {};
      struct name_separator : pad< one< ':' >, ws > {};
      struct value_separator : pad< one< ',' >, ws > {};

      struct false_ : string< 'f', 'a', 'l', 's', 'e' > {};
      struct null : string< 'n', 'u', 'l', 'l' > {};
      struct true_ : string< 't', 'r', 'u', 'e' > {};

      struct digits : plus< abnf::DIGIT > {};
      struct exp : seq< one< 'e', 'E' >, opt< one< '-', '+' > >, must< digits > > {};
      struct frac : if_must< one< '.' >, digits > {};
      struct int_ : sor< one< '0' >, digits > {};
      struct number : seq< opt< one< '-' > >, int_, opt< frac >, opt< exp > > {};

      struct xdigit : abnf::HEXDIG {};
      struct unicode : seq< one< 'u' >, rep< 4, must< xdigit > > > {};
      struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
      struct escaped : sor< escaped_char, unicode > {};
      struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
      struct char_ : if_then_else< one< '\\' >, must< escaped >, unescaped > {};
      struct string_content : until< at< one< '"' > >, must< char_ > > {};
      struct string : seq< one< '"' >, must< string_content >, any >
      {
         using content = string_content;
      };
      struct key : string {};

      struct value;

      struct member : if_must< key, name_separator, value > {};
      struct object : seq< begin_object, opt< list_must< member, value_separator > >, must< end_object > > {};

      struct array : seq< begin_array, opt< list_must< value, value_separator > >, must< end_array > > {};

      struct value : sor< false_, null, true_, object, array, number, string > {};

      struct text : pad< value, ws > {};

   } // json

} // pegtl

#endif
