// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UNICODE_HPP
#define TAO_PEGTL_UNICODE_HPP

#include "config.hpp"

#include "internal/peek_unicode.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::unicode
{
   // clang-format off
   struct cr : internal::one< internal::peek_unicode, '\r' > {};
   struct lf : internal::one< internal::peek_unicode, '\n' > {};

   struct ls : internal::one< internal::peek_unicode, char32_t( 0x2028 ) > {};
   struct nel : internal::one< internal::peek_unicode, char32_t( 0x85 ) > {};
   struct ps : internal::one< internal::peek_unicode, char32_t( 0x2029 ) > {};

   struct cr_lf : internal::one< internal::peek_unicode, '\r', '\n' > { using eol_lazy_peek = internal::peek_unicode; };
   struct crlf : internal::seq_one< internal::peek_unicode, '\r', '\n' > { using eol_lazy_peek = internal::peek_unicode; };
   struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > { using eol_lazy_peek = internal::peek_unicode; };
   struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > { using eol_lazy_peek = internal::peek_unicode; };
   struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > { using eol_lazy_peek = internal::peek_unicode; };

   struct eol1 : internal::one< internal::peek_unicode, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > { using eol_lazy_peek = internal::peek_unicode; };
   struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > { using eol_lazy_peek = internal::peek_unicode; };

   struct any : internal::any< internal::peek_unicode > {};
   struct bom : internal::one< internal::peek_unicode, 0xfeff > {};
   template< unsigned Count > struct many : internal::many< Count, internal::peek_unicode > {};
   template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_unicode, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_unicode, Lo, Hi > {};
   template< char32_t... Cs > struct one : internal::one< internal::peek_unicode, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_unicode, Lo, Hi > {};
   template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_unicode, Cs... > {};
   template< char32_t... Cs > struct string : internal::unicode_string< Cs... > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::unicode

#endif
