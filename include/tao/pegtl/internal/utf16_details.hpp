// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF16_DETAILS_HPP
#define TAO_PEGTL_INTERNAL_UTF16_DETAILS_HPP

#include "../config.hpp"

#include "utf16_constants.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_utf16_surrogate( const char32_t t ) noexcept
   {
      return ( utf16_min_surrogate <= t ) && ( t <= utf16_max_surrogate );
   }

   [[nodiscard]] constexpr bool is_utf16_high_surrogate( const char32_t t ) noexcept
   {
      return ( utf16_min_high_surrogate <= t ) && ( t <= utf16_max_high_surrogate );
   }

   [[nodiscard]] constexpr bool is_utf16_low_surrogate( const char32_t t ) noexcept
   {
      return ( utf16_min_low_surrogate <= t ) && ( t <= utf16_max_low_surrogate );
   }

   [[nodiscard]] constexpr char32_t utf16_compose( const char16_t high, const char16_t low ) noexcept
   {
      return ( ( char32_t( high & 0x03ff ) << 10 ) | char32_t( low & 0x03ff ) ) + 0x10000;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
