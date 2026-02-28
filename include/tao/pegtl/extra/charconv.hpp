// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_CHARCONV_HPP
#define TAO_PEGTL_EXTRA_CHARCONV_HPP

#include <cstdint>

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"

#include "internal/charconv.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct from_chars_raw : internal::from_chars< 0 > {};
   struct from_chars_bin : internal::from_chars< 2 > {};
   struct from_chars_oct : internal::from_chars< 8 > {};
   struct from_chars_dec : internal::from_chars< 10 > {};
   struct from_chars_hex : internal::from_chars< 16 > {};
   // clang-format on

   template< typename Name, std::uint8_t Base >
   struct analyze_traits< Name, internal::from_chars_rule< Base > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
