// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF32_DETAILS_HPP
#define TAO_PEGTL_INTERNAL_UTF32_DETAILS_HPP

#include "../config.hpp"

#include "utf16_details.hpp"
#include "unicode_constants.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_utf32_codepoint( const char32_t t ) noexcept
   {
      return ( t <= maximum_codepoint ) && ( !is_utf16_surrogate( t ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
