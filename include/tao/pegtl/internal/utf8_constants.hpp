// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_CONSTANTS_HPP
#define TAO_PEGTL_INTERNAL_UTF8_CONSTANTS_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   inline constexpr char32_t utf8_min_length_1 = 0;
   inline constexpr char32_t utf8_max_length_1 = 0x00007f;
   inline constexpr char32_t utf8_min_length_2 = 0x000080;
   inline constexpr char32_t utf8_max_length_2 = 0x0007ff;
   inline constexpr char32_t utf8_min_length_3 = 0x000800;
   inline constexpr char32_t utf8_max_length_3 = 0x00ffff;
   inline constexpr char32_t utf8_min_length_4 = 0x010000;
   inline constexpr char32_t utf8_max_length_4 = 0x10ffff;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
