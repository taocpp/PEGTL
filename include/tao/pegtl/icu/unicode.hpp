// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ICU_UNICODE_HPP
#define TAO_PEGTL_ICU_UNICODE_HPP

#include "../config.hpp"
#include "../unicode.hpp"

#include "internal.hpp"

#include "../internal/peek_unicode.hpp"

namespace TAO_PEGTL_NAMESPACE::unicode::icu
{
   template< UProperty P, bool V = true >
   struct binary_property
      : internal::icu::binary_property< internal::peek_unicode, P, V >
   {};

   template< UProperty P, int V >
   struct property_value
      : internal::icu::property_value< internal::peek_unicode, P, V >
   {};

#include "icu_rules.ipp"

}  // namespace TAO_PEGTL_NAMESPACE::unicode::icu

#endif  // FCOV_EXCL_FILE
