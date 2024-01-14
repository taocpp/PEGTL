// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_STRING_HPP
#define TAO_PEGTL_INTERNAL_UTF8_STRING_HPP

#include "../config.hpp"

#include "direct_string.hpp"
#include "unicode_to_utf8_string.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char32_t... Cs >
   using utf8_string = typename unicode_to_utf8_string< direct_string, Cs... >::type;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
