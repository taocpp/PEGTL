// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EOLS_HPP
#define TAO_PEGTL_EOLS_HPP

#include "config.hpp"

#include "internal/ascii_string.hpp"
#include "internal/endian.hpp"
#include "internal/one.hpp"
#include "internal/peek_ascii8.hpp"
#include "internal/sor.hpp"

namespace TAO_PEGTL_NAMESPACE::eols
{
   namespace rule
   {
      // clang-format off
      struct cr : internal::one< internal::peek_ascii8, '\r' > {};
      struct lf : internal::one< internal::peek_ascii8, '\n' > {};
      struct crlf : internal::ascii_string< internal::identity_endian, '\r', '\n' > {};
      struct lfcr : internal::ascii_string< internal::identity_endian, '\n', '\r' > {};
      struct cr_lf : internal::one< internal::peek_ascii8, '\r', '\n' > {};
      struct cr_crlf : internal::sor< internal::ascii_string< internal::identity_endian, '\r', '\n' >, internal::one< internal::peek_ascii8, '\r' > > {};
      struct lf_crlf : internal::sor< internal::one< internal::peek_ascii8, '\n' >, internal::ascii_string< internal::identity_endian, '\r', '\n' > > {};
      struct cr_lf_crlf : internal::sor< internal::ascii_string< internal::identity_endian, '\r', '\n' >, internal::one< internal::peek_ascii8, '\r', '\n' > > {};
      // clang-format on
   }  // namespace rule

   namespace scan
   {
      // clang-format off
      struct cr : internal::one< internal::peek_ascii8, '\r' > { static constexpr char eol_char = '\r'; };
      struct lf : internal::one< internal::peek_ascii8, '\n' > { static constexpr char eol_char = '\n'; };
      struct cr_crlf : internal::sor< internal::ascii_string< internal::identity_endian, '\r', '\n' >, internal::one< internal::peek_ascii8, '\r' > > { static constexpr char eol_char = '\r'; };
      struct lf_crlf : internal::sor< internal::one< internal::peek_ascii8, '\n' >, internal::ascii_string< internal::identity_endian, '\r', '\n' > > { static constexpr char eol_char = '\n'; };
      // clang-format on
   }  // namespace scan

}  // namespace TAO_PEGTL_NAMESPACE::eols

#endif
