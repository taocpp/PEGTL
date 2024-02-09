// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF8_HPP
#define TAO_PEGTL_UTF8_HPP

#include "config.hpp"

#include "internal/ascii_string.hpp"
#include "internal/peek_direct.hpp"
#include "internal/peek_utf8.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::utf8
{
   // clang-format off
   struct cr : internal::one< internal::peek_char, '\r' > {};
   struct lf : internal::one< internal::peek_char, '\n' > {};

   struct ls : internal::one< internal::peek_utf8, char32_t( 0x2028 ) > {};
   struct nel : internal::one< internal::peek_utf8, char32_t( 0x85 ) > {};
   struct ps : internal::one< internal::peek_utf8, char32_t( 0x2029 ) > {};

   struct cr_lf : internal::one< internal::peek_char, '\r', '\n' > { using eol_lazy_peek = internal::peek_utf8; };  // TODO: Can peek_char be used? If yes, is peek_char better (faster)?
   struct crlf : internal::ascii_string< '\r', '\n' > { using eol_lazy_peek = internal::peek_utf8; };
   struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > { using eol_lazy_peek = internal::peek_utf8; };
   struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > { using eol_lazy_peek = internal::peek_utf8; };
   struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > { using eol_lazy_peek = internal::peek_utf8; };

   struct eol1 : internal::one< internal::peek_utf8, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > { using eol_lazy_peek = internal::peek_utf8; };
   struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > { using eol_lazy_peek = internal::peek_utf8; };

   struct any : internal::any< internal::peek_utf8 > {};
   struct bom : internal::one< internal::peek_utf8, 0xfeff > {};  // Lemon curry?
   template< unsigned Count > struct many : internal::many< Count, internal::peek_utf8 > {};
   template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf8, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf8, Lo, Hi > {};
   template< char32_t... Cs > struct one : internal::one< internal::peek_utf8, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf8, Lo, Hi > {};
   template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf8, Cs... > {};
   template< char32_t... Cs > struct string : internal::utf8_string< Cs... > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::utf8

#endif
