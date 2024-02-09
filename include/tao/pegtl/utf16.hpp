// Copyright (c) 2015-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF16_HPP
#define TAO_PEGTL_UTF16_HPP

#include "config.hpp"

#include "internal/peek_utf16.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf16_be
   {
      // clang-format off
      struct cr : internal::one< internal::peek_utf16_be, '\r' > {};
      struct lf : internal::one< internal::peek_utf16_be, '\n' > {};

      struct ls : internal::one< internal::peek_utf16_be, char32_t( 0x2028 ) > {};
      struct nel : internal::one< internal::peek_utf16_be, char32_t( 0x85 ) > {};
      struct ps : internal::one< internal::peek_utf16_be, char32_t( 0x2029 ) > {};

      struct cr_lf : internal::one< internal::peek_utf16_be, '\r', '\n' > { using eol_lazy_peek = internal::peek_utf16_be; };
      struct crlf : internal::seq_one< internal::peek_utf16_be, '\r', '\n' > { using eol_lazy_peek = internal::peek_utf16_be; };
      struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > { using eol_lazy_peek = internal::peek_utf16_be; };
      struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > { using eol_lazy_peek = internal::peek_utf16_be; };
      struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > { using eol_lazy_peek = internal::peek_utf16_be; };

      struct eol1 : internal::one< internal::peek_utf16_be, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > { using eol_lazy_peek = internal::peek_utf16_be; };
      struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > { using eol_lazy_peek = internal::peek_utf16_be; };

      struct any : internal::any< internal::peek_utf16_be > {};
      struct bom : internal::one< internal::peek_utf16_be, 0xfeff > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_utf16_be > {};
      template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf16_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf16_be, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::peek_utf16_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf16_be, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf16_be, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq_one< internal::peek_utf16_be, Cs... > {};
      // clang-format on

   }  // namespace utf16_be

   namespace utf16_le
   {
      // clang-format off
      struct cr : internal::one< internal::peek_utf16_le, '\r' > {};
      struct lf : internal::one< internal::peek_utf16_le, '\n' > {};

      struct ls : internal::one< internal::peek_utf16_le, char32_t( 0x2028 ) > {};
      struct nel : internal::one< internal::peek_utf16_le, char32_t( 0x85 ) > {};
      struct ps : internal::one< internal::peek_utf16_le, char32_t( 0x2029 ) > {};

      struct cr_lf : internal::one< internal::peek_utf16_le, '\r', '\n' > { using eol_lazy_peek = internal::peek_utf16_le; };
      struct crlf : internal::seq_one< internal::peek_utf16_le, '\r', '\n' > { using eol_lazy_peek = internal::peek_utf16_le; };
      struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > { using eol_lazy_peek = internal::peek_utf16_le; };
      struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > { using eol_lazy_peek = internal::peek_utf16_le; };
      struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > { using eol_lazy_peek = internal::peek_utf16_le; };

      struct eol1 : internal::one< internal::peek_utf16_le, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > { using eol_lazy_peek = internal::peek_utf16_le; };
      struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > { using eol_lazy_peek = internal::peek_utf16_le; };

      struct any : internal::any< internal::peek_utf16_le > {};
      struct bom : internal::one< internal::peek_utf16_le, 0xfeff > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_utf16_le > {};
      template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf16_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf16_le, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::peek_utf16_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf16_le, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf16_le, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq_one< internal::peek_utf16_le, Cs... > {};
      // clang-format on

   }  // namespace utf16_le

   namespace utf16 = TAO_PEGTL_ENDIAN_SUFFIXED( utf16_ );

}  // namespace TAO_PEGTL_NAMESPACE

#endif
