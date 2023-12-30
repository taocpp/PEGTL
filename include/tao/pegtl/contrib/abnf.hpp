// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_ABNF_HPP
#define TAO_PEGTL_CONTRIB_ABNF_HPP

#include "../config.hpp"

#include "../internal/endian.hpp"
#include "../internal/peek_ascii8.hpp"
#include "../internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::abnf
{
   // Core ABNF rules according to RFC 5234, Appendix B

   // clang-format off
   struct ALPHA : internal::ranges< internal::peek_ascii8, 'a', 'z', 'A', 'Z' > {};
   struct BIT : internal::one< internal::peek_ascii8, '0', '1' > {};
   struct CHAR : internal::range< internal::peek_ascii8, static_cast< char >( 1 ), static_cast< char >( 127 ) > {};
   struct CR : internal::one< internal::peek_ascii8, '\r' > {};
   struct CRLF : internal::ascii_string< internal::identity_endian, '\r', '\n' > {};
   struct CTL : internal::ranges< internal::peek_ascii8, static_cast< char >( 0 ), static_cast< char >( 31 ), static_cast< char >( 127 ) > {};
   struct DIGIT : internal::range< internal::peek_ascii8, '0', '9' > {};
   struct DQUOTE : internal::one< internal::peek_ascii8, '"' > {};
   struct HEXDIG : internal::ranges< internal::peek_ascii8, '0', '9', 'a', 'f', 'A', 'F' > {};
   struct HTAB : internal::one< internal::peek_ascii8, '\t' > {};
   struct LF : internal::one< internal::peek_ascii8, '\n' > {};
   struct LWSP : internal::star< internal::sor< internal::ascii_string< internal::identity_endian, '\r', '\n' >, internal::one< internal::peek_ascii8, ' ', '\t' > >, internal::one< internal::peek_ascii8, ' ', '\t' > > {};
   struct OCTET : internal::any< internal::peek_ascii8 > {};
   struct SP : internal::one< internal::peek_ascii8, ' ' > {};
   struct VCHAR : internal::range< internal::peek_ascii8, static_cast< char >( 33 ), static_cast< char >( 126 ) > {};
   struct WSP : internal::one< internal::peek_ascii8, ' ', '\t' > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::abnf

#endif
