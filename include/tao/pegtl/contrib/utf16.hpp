// Copyright (c) 2015-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_UTF16_HPP
#define TAO_PEGTL_CONTRIB_UTF16_HPP

#include "../config.hpp"

#include "../internal/result_on_found.hpp"
#include "../internal/rules.hpp"

#include "internal/peek_utf16.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf16_be
   {
      // clang-format off
      struct any : internal::any< internal::peek_utf16_be > {};
      struct bom : internal::one< internal::result_on_found::success, internal::peek_utf16_be, 0xfeff > {};
      template< char32_t... Cs > struct not_one : internal::one< internal::result_on_found::failure, internal::peek_utf16_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::range< internal::result_on_found::failure, internal::peek_utf16_be, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::result_on_found::success, internal::peek_utf16_be, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::result_on_found::success, internal::peek_utf16_be, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf16_be, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::success, internal::peek_utf16_be, Cs >... > {};
      // clang-format on

   }  // namespace utf16_be

   namespace utf16_le
   {
      // clang-format off
      struct any : internal::any< internal::peek_utf16_le > {};
      struct bom : internal::one< internal::result_on_found::success, internal::peek_utf16_le, 0xfeff > {};
      template< char32_t... Cs > struct not_one : internal::one< internal::result_on_found::failure, internal::peek_utf16_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::range< internal::result_on_found::failure, internal::peek_utf16_le, Lo, Hi > {};
      template< char32_t... Cs > struct one : internal::one< internal::result_on_found::success, internal::peek_utf16_le, Cs... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::result_on_found::success, internal::peek_utf16_le, Lo, Hi > {};
      template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf16_le, Cs... > {};
      template< char32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::success, internal::peek_utf16_le, Cs >... > {};
      // clang-format on

   }  // namespace utf16_le

#if defined( _WIN32 ) && !defined( __MINGW32__ ) && !defined( __CYGWIN__ )
   namespace utf16 = utf16_le;
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
   namespace utf16 = utf16_le;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   namespace utf16 = utf16_be;
#else
#error Unknown endianness.
#endif

}  // namespace TAO_PEGTL_NAMESPACE

#endif
