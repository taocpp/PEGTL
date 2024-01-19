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
