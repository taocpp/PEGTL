// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ASCII_HH
#define PEGTL_ASCII_HH

#include <type_traits>

#include "internal/rules.hh"

namespace pegtl
{
   inline namespace ascii
   {
      struct alnum : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > {};
      struct alpha : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > {};
      struct any : internal::any< internal::peek_char > {};
      template< char ... Cs > struct at_not_one : internal::at_one< false, internal::peek_char, Cs ... > {};
      template< char Lo, char Hi > struct at_not_range : internal::at_range< false, internal::peek_char, Lo, Hi > {};
      template< char ... Cs > struct at_one : internal::at_one< true, internal::peek_char, Cs ... > {};
      template< char Lo, char Hi > struct at_range : internal::at_range< true, internal::peek_char, Lo, Hi > {};
      struct blank : internal::one< true, internal::peek_char, ' ', '\t' > {};
      struct digit : internal::range< true, internal::peek_char, '0', '9' > {};
      struct eol : internal::one< true, internal::peek_char, '\n' > {};
      struct eolf : internal::eolf {};
      struct identifier_first : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '_' > {};
      struct identifier_other : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > {};
      struct identifier : internal::seq< identifier_first, internal::star< identifier_other > > {};
      template< char ... Cs > struct istring : internal::istring< Cs ... > {};
      struct lower : internal::range< true, internal::peek_char, 'a', 'z' > {};
      template< char ... Cs > struct not_one : internal::one< false, internal::peek_char, Cs ... > {};
      template< char Lo, char Hi > struct not_range : internal::range< false, internal::peek_char, Lo, Hi > {};
      struct nul : internal::one< true, internal::peek_char, char( 0 ) > {};
      template< char ... Cs > struct one : internal::one< true, internal::peek_char, Cs ... > {};
      struct print : internal::range< true, internal::peek_char, char( 32 ), char( 126 ) > {};
      template< char Lo, char Hi > struct range : internal::range< true, internal::peek_char, Lo, Hi > {};
      template< char ... Cs > struct ranges : internal::ranges< internal::peek_char, Cs ... > {};
      struct seven : internal::range< true, internal::peek_char, char( 0 ), char( 127 ) > {};
      struct shebang : internal::if_must< internal::string< '#', '!' >, internal::until< internal::eolf > > {};
      struct space : internal::one< true, internal::peek_char, ' ', '\n', '\r', '\t', '\v', '\f' > {};
      template< char ... Cs > struct string : internal::string< Cs ... > {};
      template< char C > struct two : internal::string< C, C > {};
      struct upper : internal::range< true, internal::peek_char, 'A', 'Z' > {};
      struct xdigit : internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > {};

   } // ascii

} // pegtl

#endif
