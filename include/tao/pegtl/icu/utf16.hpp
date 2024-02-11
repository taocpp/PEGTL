// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ICU_UTF16_HPP
#define TAO_PEGTL_ICU_UTF16_HPP

#include "../config.hpp"
#include "../utf16.hpp"

#include "internal.hpp"

#include "../internal/peek_utf16.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf16_be::icu
   {
      template< UProperty P, bool V = true >
      struct binary_property
         : internal::icu::binary_property< internal::peek_utf16_be, P, V >
      {};

      template< UProperty P, int V >
      struct property_value
         : internal::icu::property_value< internal::peek_utf16_be, P, V >
      {};

#include "icu_rules.ipp"

   }  // namespace utf16_be::icu

   namespace utf16_le::icu
   {
      template< UProperty P, bool V = true >
      struct binary_property
         : internal::icu::binary_property< internal::peek_utf16_le, P, V >
      {};

      template< UProperty P, int V >
      struct property_value
         : internal::icu::property_value< internal::peek_utf16_le, P, V >
      {};

#include "icu_rules.ipp"

   }  // namespace utf16_le::icu

}  // namespace TAO_PEGTL_NAMESPACE

#endif
