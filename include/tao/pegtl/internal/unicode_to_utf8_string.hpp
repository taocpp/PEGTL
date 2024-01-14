// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_STRING_HPP
#define TAO_PEGTL_INTERNAL_UNICODE_TO_UTF8_STRING_HPP

#include "../config.hpp"

#include "unicode_to_utf8_char.hpp"
#include "value_list_concat.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< char... > class String, char32_t... Cs >
   struct unicode_to_utf8_string
   {
      using type = value_list_concat_t< typename unicode_to_utf8_char< String, Cs >::type... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
