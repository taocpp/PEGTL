// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_ASCII_UTILITY_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_ASCII_UTILITY_HPP

#include "../../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_ascii_digit( const char c ) noexcept
   {
      return ( '0' <= c ) && ( c <= '9' );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
