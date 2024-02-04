// Copyright (c) 2022-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ENUMS_HPP
#define TAO_PEGTL_ENUMS_HPP

#include "config.hpp"

#include "internal/endian.hpp"
#include "internal/peek_endian.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace enums_be
   {
      // clang-format off
      template< auto E, decltype( E )... Es > struct not_one : internal::not_one< internal::peek_endian< decltype( E ), internal::big_endian >, E, Es... > {};
      template< auto Lo, decltype( Lo ) Hi > struct not_range : internal::not_range< internal::peek_endian< decltype( Lo ), internal::big_endian >, Lo, Hi > {};
      template< auto E, decltype( E )... Es > struct one : internal::one< internal::peek_endian< decltype( E ), internal::big_endian >, E, Es... > {};
      template< auto Lo, decltype( Lo ) Hi > struct range : internal::range< internal::peek_endian< decltype( Lo ), internal::big_endian >, Lo, Hi > {};
      template< auto E, decltype( E )... Es > struct ranges : internal::ranges< internal::peek_endian< decltype( E ), internal::big_endian >, E, Es... > {};
      template< auto E, decltype( E )... Es > struct string : internal::seq_one< internal::peek_endian< decltype( E ), internal::big_endian >, E, Es... > {};
      // clang-format on

   }  // namespace enums_be

   namespace enums_le
   {
      // clang-format off
      template< auto E, decltype( E )... Es > struct not_one : internal::not_one< internal::peek_endian< decltype( E ), internal::little_endian >, E, Es... > {};
      template< auto Lo, decltype( Lo ) Hi > struct not_range : internal::not_range< internal::peek_endian< decltype( Lo ), internal::little_endian >, Lo, Hi > {};
      template< auto E, decltype( E )... Es > struct one : internal::one< internal::peek_endian< decltype( E ), internal::little_endian >, E, Es... > {};
      template< auto Lo, decltype( Lo ) Hi > struct range : internal::range< internal::peek_endian< decltype( Lo ), internal::little_endian >, Lo, Hi > {};
      template< auto E, decltype( E )... Es > struct ranges : internal::ranges< internal::peek_endian< decltype( E ), internal::little_endian >, E, Es... > {};
      template< auto E, decltype( E )... Es > struct string : internal::seq_one< internal::peek_endian< decltype( E ), internal::little_endian >, E, Es... > {};
      // clang-format on

   }  // namespace enums_le

   namespace enums = TAO_PEGTL_ENDIAN_SUFFIXED( enums_ );

}  // namespace TAO_PEGTL_NAMESPACE

#endif
