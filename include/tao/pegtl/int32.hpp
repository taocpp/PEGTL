// Copyright (c) 2018-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INT32_HPP
#define TAO_PEGTL_INT32_HPP

#include "config.hpp"

#include "internal/peek_integer.hpp"
#include "internal/result_on_found.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace int32_be
   {
      // clang-format off
      struct any : internal::any< internal::peek_int32_be > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_int32_be > {};

      template< std::int32_t... Cs > struct not_one : internal::one< internal::result_on_found::failure, internal::peek_int32_be, Cs... > {};
      template< std::int32_t Lo, std::int32_t Hi > struct not_range : internal::range< internal::result_on_found::failure, internal::peek_int32_be, Lo, Hi > {};
      template< std::int32_t... Cs > struct one : internal::one< internal::result_on_found::success, internal::peek_int32_be, Cs... > {};
      template< std::int32_t Lo, std::int32_t Hi > struct range : internal::range< internal::result_on_found::success, internal::peek_int32_be, Lo, Hi > {};
      template< std::int32_t... Cs > struct ranges : internal::ranges< internal::peek_int32_be, Cs... > {};
      template< std::int32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::success, internal::peek_int32_be, Cs >... > {};
      // clang-format on

   }  // namespace int32_be

   namespace int32_le
   {
      // clang-format off
      struct any : internal::any< internal::peek_int32_le > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_int32_le > {};

      template< std::int32_t... Cs > struct not_one : internal::one< internal::result_on_found::failure, internal::peek_int32_le, Cs... > {};
      template< std::int32_t Lo, std::int32_t Hi > struct not_range : internal::range< internal::result_on_found::failure, internal::peek_int32_le, Lo, Hi > {};
      template< std::int32_t... Cs > struct one : internal::one< internal::result_on_found::success, internal::peek_int32_le, Cs... > {};
      template< std::int32_t Lo, std::int32_t Hi > struct range : internal::range< internal::result_on_found::success, internal::peek_int32_le, Lo, Hi > {};
      template< std::int32_t... Cs > struct ranges : internal::ranges< internal::peek_int32_le, Cs... > {};
      template< std::int32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::success, internal::peek_int32_le, Cs >... > {};
      // clang-format on

   }  // namespace int32_le

}  // namespace TAO_PEGTL_NAMESPACE

#endif
