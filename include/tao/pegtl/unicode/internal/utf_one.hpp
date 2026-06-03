// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UNICODE_INTERNAL_UTF_ONE_HPP
#define TAO_PEGTL_UNICODE_INTERNAL_UTF_ONE_HPP

#include "../../config.hpp"

#include "../../internal/one.hpp"

#include "peek_utf8.hpp"
#include "utf32_to_utf8.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, char32_t... Cs >
   struct utf_one
      : one< Peek, Cs... >
   {};

   template< char32_t C >
   struct utf_one< peek_utf8, C >
      : utf32_to_utf8_t< C >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
