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

      struct exp : seq< one< 'e', 'E' >, opt< one< '-', '+' > >, plus< DIGIT > > {};
      struct frac : seq< one< '.' >, plus< DIGIT > > {};
      struct int_ : sor< one< '0' >, plus< DIGIT > > {};
      struct number : seq< opt< one< '-' > >, int_, opt< frac >, opt< exp > > {};

      struct unicode : seq< one< 'u' >, rep< 4, HEXDIG > > {};
      struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
      struct escaped : sor< escaped_char, unicode > {};
      struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
      struct char_ : if_then_else< one< '\\' >, escaped, unescaped > {};
      struct string_content : until< at< one< '"' > >, char_ > {};
      struct string : seq< one< '"' >, string_content, one< '"' > > {};

      struct value;

      struct member : seq< string, name_separator, value > {};
      struct object : seq< begin_object, opt< list< member, value_separator > >, end_object > {};

      struct array : seq< begin_array, opt< list< value, value_separator > >, end_array > {};

      struct value : sor< false_, null, true_, object, array, number, string > {};

      struct data : seq< sor< object, array >, eof > {};

   } // json

} // pegtl

#endif
