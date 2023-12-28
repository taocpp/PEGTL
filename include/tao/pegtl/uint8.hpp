// Copyright (c) 2018-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UINT8_HPP
#define TAO_PEGTL_UINT8_HPP

#include "config.hpp"

#include "internal/peek_direct.hpp"
#include "internal/peek_integer.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE::uint8
{
   // clang-format off
   struct any : internal::any< internal::peek_uint8 > {};
   template< unsigned Count > struct many : internal::many< Count, internal::peek_uint8 > {};

   template< std::uint8_t... Cs > struct not_one : internal::not_one< internal::peek_uint8, Cs... > {};
   template< std::uint8_t Lo, std::uint8_t Hi > struct not_range : internal::not_range< internal::peek_uint8, Lo, Hi > {};
   template< std::uint8_t... Cs > struct one : internal::one< internal::peek_uint8, Cs... > {};
   template< std::uint8_t Lo, std::uint8_t Hi > struct range : internal::range< internal::peek_uint8, Lo, Hi > {};
   template< std::uint8_t... Cs > struct ranges : internal::ranges< internal::peek_uint8, Cs... > {};
   template< std::uint8_t... Cs > struct string : internal::seq_one< internal::peek_uint8, Cs... > {};

   template< std::uint8_t M, std::uint8_t... Cs > struct mask_not_one : internal::not_one< internal::peek_mask_uint8< M >, Cs... > {};
   template< std::uint8_t M, std::uint8_t Lo, std::uint8_t Hi > struct mask_not_range : internal::not_range< internal::peek_mask_uint8< M >, Lo, Hi > {};
   template< std::uint8_t M, std::uint8_t... Cs > struct mask_one : internal::one< internal::peek_mask_uint8< M >, Cs... > {};
   template< std::uint8_t M, std::uint8_t Lo, std::uint8_t Hi > struct mask_range : internal::range< internal::peek_mask_uint8< M >, Lo, Hi > {};
   template< std::uint8_t M, std::uint8_t... Cs > struct mask_ranges : internal::ranges< internal::peek_mask_uint8< M >, Cs... > {};
   template< std::uint8_t M, std::uint8_t... Cs > struct mask_string : internal::seq_one< internal::peek_mask_uint8< M >, Cs... > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::uint8

#endif
