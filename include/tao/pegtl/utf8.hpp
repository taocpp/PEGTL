// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF8_HPP
#define TAO_PEGTL_UTF8_HPP

#include "config.hpp"

#include "internal/peek_utf8.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::utf8
{
   // clang-format off
   struct any : internal::any< internal::peek_utf8 > {};
   struct bom : internal::one< internal::peek_utf8, 0xfeff > {};  // Lemon curry?
   template< unsigned Count > struct many : internal::many< Count, internal::peek_utf8 > {};
   template< char32_t... Cs > struct not_one : internal::not_one< internal::peek_utf8, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct not_range : internal::not_range< internal::peek_utf8, Lo, Hi > {};
   template< char32_t... Cs > struct one : internal::one< internal::peek_utf8, Cs... > {};
   template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::peek_utf8, Lo, Hi > {};
   template< char32_t... Cs > struct ranges : internal::ranges< internal::peek_utf8, Cs... > {};
   template< char32_t... Cs > struct string : internal::seq_one< internal::peek_utf8, Cs... > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::utf8

#endif
