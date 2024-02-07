// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_ABNF_HPP
#define TAO_PEGTL_CONTRIB_ABNF_HPP

#include "../config.hpp"

#include "../internal/peek_direct.hpp"
#include "../internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::abnf
{
   // Core ABNF rules according to RFC 5234, Appendix B

   // clang-format off
   struct ALPHA : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > {};
   struct BIT : internal::one< internal::peek_char, '0', '1' > {};
   struct CHAR : internal::range< internal::peek_char, static_cast< char >( 1 ), static_cast< char >( 127 ) > {};
   struct CR : internal::one< internal::peek_char, '\r' > {};
   struct CRLF : internal::ascii_string< '\r', '\n' > {};
   struct CTL : internal::ranges< internal::peek_char, static_cast< char >( 0 ), static_cast< char >( 31 ), static_cast< char >( 127 ) > {};
   struct DIGIT : internal::range< internal::peek_char, '0', '9' > {};
   struct DQUOTE : internal::one< internal::peek_char, '"' > {};
   struct HEXDIG : internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > {};
   struct HTAB : internal::one< internal::peek_char, '\t' > {};
   struct LF : internal::one< internal::peek_char, '\n' > {};
   struct LWSP : internal::star< internal::sor< internal::ascii_string< '\r', '\n' >, internal::one< internal::peek_char, ' ', '\t' > >, internal::one< internal::peek_char, ' ', '\t' > > {};
   struct OCTET : internal::any< internal::peek_char > {};
   struct SP : internal::one< internal::peek_char, ' ' > {};
   struct VCHAR : internal::range< internal::peek_char, static_cast< char >( 33 ), static_cast< char >( 126 ) > {};
   struct WSP : internal::one< internal::peek_char, ' ', '\t' > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::abnf

#endif
