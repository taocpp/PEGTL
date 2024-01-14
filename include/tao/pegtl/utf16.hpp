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
