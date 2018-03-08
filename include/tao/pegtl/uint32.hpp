// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_UINT32_HPP
#define TAO_PEGTL_UINT32_HPP

#include "config.hpp"

#include "internal/peek_uint.hpp"
#include "internal/result_on_found.hpp"
#include "internal/rules.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace uint32_be
      {
         // clang-format off
         struct any : internal::any< internal::peek_uint32_be > {};
         template< std::uint32_t... Cs > struct not_one : internal::one< internal::result_on_found::FAILURE, internal::peek_uint32_be, Cs... > {};
         template< std::uint32_t Lo, std::uint32_t Hi > struct not_range : internal::range< internal::result_on_found::FAILURE, internal::peek_uint32_be, Lo, Hi > {};
         template< std::uint32_t... Cs > struct one : internal::one< internal::result_on_found::SUCCESS, internal::peek_uint32_be, Cs... > {};
         template< std::uint32_t Lo, std::uint32_t Hi > struct range : internal::range< internal::result_on_found::SUCCESS, internal::peek_uint32_be, Lo, Hi > {};
         template< std::uint32_t... Cs > struct ranges : internal::ranges< internal::peek_uint32_be, Cs... > {};
         template< std::uint32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::SUCCESS, internal::peek_uint32_be, Cs >... > {};
         // clang-format on

      }  // namespace uint32_be

      namespace uint32_le
      {
         // clang-format off
         struct any : internal::any< internal::peek_uint32_le > {};
         template< std::uint32_t... Cs > struct not_one : internal::one< internal::result_on_found::FAILURE, internal::peek_uint32_le, Cs... > {};
         template< std::uint32_t Lo, std::uint32_t Hi > struct not_range : internal::range< internal::result_on_found::FAILURE, internal::peek_uint32_le, Lo, Hi > {};
         template< std::uint32_t... Cs > struct one : internal::one< internal::result_on_found::SUCCESS, internal::peek_uint32_le, Cs... > {};
         template< std::uint32_t Lo, std::uint32_t Hi > struct range : internal::range< internal::result_on_found::SUCCESS, internal::peek_uint32_le, Lo, Hi > {};
         template< std::uint32_t... Cs > struct ranges : internal::ranges< internal::peek_uint32_le, Cs... > {};
         template< std::uint32_t... Cs > struct string : internal::seq< internal::one< internal::result_on_found::SUCCESS, internal::peek_uint32_le, Cs >... > {};
         // clang-format on

      }  // namespace uint32_le

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
