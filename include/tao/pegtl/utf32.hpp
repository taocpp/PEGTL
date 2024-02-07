// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF32_HPP
#define TAO_PEGTL_UTF32_HPP

#include "config.hpp"

#include "internal/peek_utf32.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf32_be
   {
      // clang-format off
      struct cr : internal::one< internal::peek_utf32_be, '\r' > {};
      struct lf : internal::one< internal::peek_utf32_be, '\n' > {};

      struct ls : internal::one< internal::peek_utf32_be, char32_t( 0x2028 ) > {};
      struct nel : internal::one< internal::peek_utf32_be, char32_t( 0x85 ) > {};
      struct ps : internal::one< internal::peek_utf32_be, char32_t( 0x2029 ) > {};

      struct cr_lf : internal::one< internal::peek_utf32_be, '\r', '\n' > {};
      struct crlf : internal::seq_one< internal::peek_utf32_be, '\r', '\n' > {};
      struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > {};
      struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > {};
      struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > {};

      struct eol1 : internal::one< internal::peek_utf32_be, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > {};
      struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > {};

      struct any : internal::any< internal::peek_utf32_be > {};
      struct bom : internal::one< internal::peek_utf32_be, 0xfeff > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_utf32_be > {};
      template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf32_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf32_be, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::peek_utf32_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf32_be, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf32_be, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq_one< internal::peek_utf32_be, Cs... > {};
      // clang-format on

   }  // namespace utf32_be

   namespace utf32_le
   {
      // clang-format off
      struct cr : internal::one< internal::peek_utf32_le, '\r' > {};
      struct lf : internal::one< internal::peek_utf32_le, '\n' > {};

      struct ls : internal::one< internal::peek_utf32_le, char32_t( 0x2028 ) > {};
      struct nel : internal::one< internal::peek_utf32_le, char32_t( 0x85 ) > {};
      struct ps : internal::one< internal::peek_utf32_le, char32_t( 0x2029 ) > {};

      struct cr_lf : internal::one< internal::peek_utf32_le, '\r', '\n' > {};
      struct crlf : internal::seq_one< internal::peek_utf32_le, '\r', '\n' > {};
      struct cr_crlf : internal::sor< crlf::rule_t, cr::rule_t > {};
      struct cr_lf_crlf : internal::sor< crlf::rule_t, cr_lf::rule_t > {};
      struct lf_crlf : internal::sor< lf::rule_t, crlf::rule_t > {};

      struct eol1 : internal::one< internal::peek_utf32_le, char32_t( '\r' ), char32_t( '\n' ), char32_t( '\v' ), char32_t( '\f' ), char32_t( 0x85 ), char32_t( 0x2028 ), char32_t( 0x2029 ) > {};
      struct eolu : internal::sor< crlf::rule_t, eol1::rule_t > {};

      struct any : internal::any< internal::peek_utf32_le > {};
      struct bom : internal::one< internal::peek_utf32_le, 0xfeff > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_utf32_le > {};
      template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf32_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf32_le, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::peek_utf32_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf32_le, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf32_le, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq_one< internal::peek_utf32_le, Cs... > {};
      // clang-format on

   }  // namespace utf32_le

   namespace utf32 = TAO_PEGTL_ENDIAN_SUFFIXED( utf32_ );

}  // namespace TAO_PEGTL_NAMESPACE

#endif
