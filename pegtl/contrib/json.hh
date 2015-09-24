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

      struct false_ : pegtl_string_t( "false" ) {};
      struct null : pegtl_string_t( "null" ) {};
      struct true_ : pegtl_string_t( "true" ) {};

      struct digits : plus< abnf::DIGIT > {};
      struct exp : seq< one< 'e', 'E' >, opt< one< '-', '+' > >, must< digits > > {};
      struct frac : if_must< one< '.' >, digits > {};
      struct int_ : sor< one< '0' >, digits > {};
      struct number : seq< opt< one< '-' > >, int_, opt< frac >, opt< exp > > {};

      struct xdigit : abnf::HEXDIG {};
      struct unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
      struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
      struct escaped : sor< escaped_char, unicode > {};
      struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
      struct char_ : if_then_else< one< '\\' >, must< escaped >, unescaped > {};

      struct string_content : until< at< one< '"' > >, must< char_ > > {};
      struct string : seq< one< '"' >, must< string_content >, any >
      {
         using content = string_content;
      };

      struct key_content : until< at< one< '"' > >, must< char_ > > {};
      struct key : seq< one< '"' >, must< key_content >, any >
      {
         using content = key_content;
      };

      struct value;

      struct array_content : opt< list_must< value, value_separator > > {};
      struct array : seq< begin_array, array_content, must< end_array > >
      {
         using content = array_content;
      };

      struct member : if_must< key, name_separator, value > {};
      struct object_content : opt< list_must< member, value_separator > > {};
      struct object : seq< begin_object, object_content, must< end_object > >
      {
         using content = object_content;
      };

      struct value : sor< string, number, object, array, false_, true_, null > {};
      struct text : pad< value, ws > {};

   } // json

} // pegtl

#endif
