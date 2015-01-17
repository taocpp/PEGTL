// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_JSON_HH
#define PEGTL_CONTRIB_JSON_HH

#include "../rules.hh"
#include "../utf8.hh"
#include "../make.hh"
#include "abnf.hh"

namespace pegtl
{
   namespace json
   {
      // JSON grammar according to RFC 4627 (for UTF-8 encoded JSON only).

      using namespace abnf;

      struct ws : one< ' ', '\t', '\n', '\r' > {};

      struct begin_array : pad< one< '[' >, ws > {};
      struct begin_object : pad< one< '{' >, ws > {};
      struct end_array : pad< one< ']' >, ws > {};
      struct end_object : pad< one< '}' >, ws > {};
      struct name_separator : pad< one< ':' >, ws > {};
      struct value_separator : pad< one< ',' >, ws > {};

      constexpr const char s_false[] = "false";
      constexpr const char s_null[] = "null";
      constexpr const char s_true[] = "true";

      struct false_ : make_string< s_false > {};
      struct null : make_string< s_null > {};
      struct true_ : make_string< s_true > {};

      struct exp : if_must< one< 'e', 'E' >, opt< one< '-', '+' > >, plus< DIGIT > > {};
      struct frac : if_must< one< '.' >, plus< DIGIT > > {};
      struct int_ : sor< one< '0' >, plus< DIGIT > > {};
      struct number : seq< opt< one< '-' > >, int_, opt< frac >, opt< exp > > {};

      struct unicode : if_must< one< 'u' >, rep< 4, HEXDIG > > {};
      struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
      struct escaped : if_must< one< '\\' >, sor< escaped_char, unicode > > {};
      struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
      struct char_ : sor< escaped, unescaped > {};
      struct string_content : until< at_one< '"' >, char_ > {};
      struct string : if_must< one< '"' >, string_content, one< '"' > > {};

      struct value;

      struct member : if_must< string, name_separator, value > {};
      struct object : if_must< begin_object, opt< list_must< member, value_separator > >, end_object > {};

      struct array : if_must< begin_array, opt< list_must< value, value_separator > >, end_array > {};

      struct value : sor< false_, null, true_, object, array, number, string > {};

      struct data : seq< sor< object, array >, eof > {};

   } // json

} // pegtl

#endif
