// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INT16_HPP
#define TAO_PEGTL_INT16_HPP

#include "config.hpp"

#include "internal/peek_integer.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace int16_be
   {
      // clang-format off
      struct any : internal::any< internal::peek_int16_be > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_int16_be > {};

      template< std::int16_t... Cs > struct not_one : internal::not_one< internal::peek_int16_be, Cs... > {};
      template< std::int16_t Lo, std::int16_t Hi > struct not_range : internal::not_range< internal::peek_int16_be, Lo, Hi > {};
      template< std::int16_t... Cs > struct one : internal::one< internal::peek_int16_be, Cs... > {};
      template< std::int16_t Lo, std::int16_t Hi > struct range : internal::range< internal::peek_int16_be, Lo, Hi > {};
      template< std::int16_t... Cs > struct ranges : internal::ranges< internal::peek_int16_be, Cs... > {};
      template< std::int16_t... Cs > struct string : internal::seq_one< internal::peek_int16_be, Cs... > {};
      // clang-format on
   }  // namespace int16_be

   namespace int16_le
   {
      // clang-format off
      struct any : internal::any< internal::peek_int16_le > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_int16_le > {};

      template< std::int16_t... Cs > struct not_one : internal::not_one< internal::peek_int16_le, Cs... > {};
      template< std::int16_t Lo, std::int16_t Hi > struct not_range : internal::not_range< internal::peek_int16_le, Lo, Hi > {};
      template< std::int16_t... Cs > struct one : internal::one< internal::peek_int16_le, Cs... > {};
      template< std::int16_t Lo, std::int16_t Hi > struct range : internal::range< internal::peek_int16_le, Lo, Hi > {};
      template< std::int16_t... Cs > struct ranges : internal::ranges< internal::peek_int16_le, Cs... > {};
      template< std::int16_t... Cs > struct string : internal::seq_one< internal::peek_int16_le, Cs... > {};
      // clang-format on
   }  // namespace int16_le

}  // namespace TAO_PEGTL_NAMESPACE

#endif
