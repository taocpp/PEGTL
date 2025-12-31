// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UNICODE_ICU8_HPP
#define TAO_PEGTL_UNICODE_ICU8_HPP

#include "../config.hpp"

#include "internal/icu_binary_property.hpp"
#include "internal/icu_property_value.hpp"
#include "internal/peek_utf8.hpp"

namespace TAO_PEGTL_NAMESPACE::utf8::icu
{
   template< UProperty P, bool V = true >
   struct binary_property
      : internal::icu_binary_property< internal::peek_utf8, P, V >
   {};

   template< UProperty P, int V >
   struct property_value
      : internal::icu_property_value< internal::peek_utf8, P, V >
   {};

#include "internal/icu_rules.ipp"

}  // namespace TAO_PEGTL_NAMESPACE::utf8::icu

// FCOV_EXCL_FILE
#endif
