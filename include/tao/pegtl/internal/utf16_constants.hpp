// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF16_CONSTANTS_HPP
#define TAO_PEGTL_INTERNAL_UTF16_CONSTANTS_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   inline constexpr char32_t utf16_min_surrogate = 0xd800;
   inline constexpr char32_t utf16_max_surrogate = 0xdfff;

   inline constexpr char32_t utf16_min_high_surrogate = 0xd800;
   inline constexpr char32_t utf16_max_high_surrogate = 0xdbff;
   inline constexpr char32_t utf16_min_low_surrogate = 0xdc00;
   inline constexpr char32_t utf16_max_low_surrogate = 0xdfff;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif  // FCOV_EXCL_FILE
