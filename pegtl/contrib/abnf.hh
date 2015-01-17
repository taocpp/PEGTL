// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_ABNF_HH
#define PEGTL_CONTRIB_ABNF_HH

#include "../internal/rules.hh"

namespace pegtl
{
   namespace abnf
   {
      // Core ABNF rules according to RFC 5234.

      struct ALPHA : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > {};
      struct BIT : internal::one< true, internal::peek_char, '0', '1' > {};
      struct CHAR : internal::range< true, internal::peek_char, char( 1 ), char( 127 ) > {};
      struct CR : internal::one< true, internal::peek_char, '\r' > {};
      struct CRLF : internal::string< '\r', '\n' > {};
      struct CTL : internal::ranges< internal::peek_char, char( 0 ), char( 31 ), char( 127 ) > {};
      struct DIGIT : internal::range< true, internal::peek_char, '0', '9' > {};
      struct DQUOTE : internal::one< true, internal::peek_char, '"' > {};
      struct HEXDIG : internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > {};
      struct HTAB : internal::one< true, internal::peek_char, '\t' > {};
      struct LF : internal::one< true, internal::peek_char, '\n' > {};
      struct LWSP : internal::star< internal::sor< internal::string< '\r', '\n' >, internal::one< true, internal::peek_char, ' ', '\t' > >, internal::one< true, internal::peek_char, ' ', '\t' > > {};
      struct OCTET : internal::any< internal::peek_char > {};
      struct SP : internal::one< true, internal::peek_char, ' ' > {};
      struct VCHAR : internal::range< true, internal::peek_char, char( 33 ), char( 126 ) > {};
      struct WSP : internal::one< true, internal::peek_char, ' ', '\t' > {};

   } // abnf

} // pegtl

#endif
