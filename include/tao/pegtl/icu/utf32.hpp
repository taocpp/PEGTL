// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ICU_UTF32_HPP
#define TAO_PEGTL_ICU_UTF32_HPP

#include "../config.hpp"
#include "../utf32.hpp"

#include "internal.hpp"

#include "../internal/peek_utf32.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf32_be::icu
   {
      template< UProperty P, bool V = true >
      struct binary_property
         : internal::icu::binary_property< internal::peek_utf32_be, P, V >
      {};

      template< UProperty P, int V >
      struct property_value
         : internal::icu::property_value< internal::peek_utf32_be, P, V >
      {};

#include "icu_rules.ipp"  // NOLINT(readability-duplicate-include)

   }  // namespace utf32_be::icu

   namespace utf32_le::icu
   {
      template< UProperty P, bool V = true >
      struct binary_property
         : internal::icu::binary_property< internal::peek_utf32_le, P, V >
      {};

      template< UProperty P, int V >
      struct property_value
         : internal::icu::property_value< internal::peek_utf32_le, P, V >
      {};

#include "icu_rules.ipp"  // NOLINT(readability-duplicate-include)

   }  // namespace utf32_le::icu

}  // namespace TAO_PEGTL_NAMESPACE

#endif
