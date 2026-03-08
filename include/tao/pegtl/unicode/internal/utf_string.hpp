// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UNICODE_INTERNAL_UTF_STRING_HPP
#define TAO_PEGTL_UNICODE_INTERNAL_UTF_STRING_HPP

#include "../../config.hpp"

#include "../../internal/seq.hpp"
#include "../../internal/terminal_aliases.hpp"

#include "peek_utf8.hpp"
#include "utf32_to_utf8.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, char32_t... Cs >
   struct utf_string
      : seq< one< Peek, Cs >... >
   {};

   template< char32_t... Cs >
   struct utf_string< peek_utf8, Cs... >
      : utf32_to_utf8_t< Cs... >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
