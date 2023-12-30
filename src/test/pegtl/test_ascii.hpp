// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_ASCII_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_ASCII_HPP

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/internal/peek_direct.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   inline namespace ascii
   {
      // clang-format off
      struct any : internal::any< internal::peek_char > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_char > {};
      template< char... Cs > struct not_one : internal::not_one< internal::peek_char, Cs... > {};
      template< char Lo, char Hi > struct not_range : internal::not_range< internal::peek_char, Lo, Hi > {};
      // clang-format on

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#endif
