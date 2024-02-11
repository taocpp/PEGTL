// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNICODE_CONSTANTS_HPP
#define TAO_PEGTL_INTERNAL_UNICODE_CONSTANTS_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   inline constexpr char32_t minimum_surrogate = 0xd800;
   inline constexpr char32_t maximum_surrogate = 0xdfff;
   inline constexpr char32_t maximum_codepoint = 0x10ffff;

   inline constexpr char32_t minimum_high_surrogate = 0xd800;
   inline constexpr char32_t maximum_high_surrogate = 0xdbff;
   inline constexpr char32_t minimum_low_surrogate = 0xdc00;
   inline constexpr char32_t maximum_low_surrogate = 0xdfff;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
