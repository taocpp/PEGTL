// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MATH_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_MATH_UTILITY_HPP

#include <cstddef>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] constexpr bool is_divisible( const std::size_t n, const std::size_t d ) noexcept
   {
      return ( n / d ) * d == n;
   }

   // Here "range" is like for the range rules, i.e. including both boundaries.

   template< typename Data >
   [[nodiscard]] constexpr bool ranges_disjoint( const Data ll, const Data lh, const Data rl, const Data rh ) noexcept
   {
      // assert( ll <= lh );
      // assert( rl <= rh );
      return ( lh < rl ) || ( rh < ll );
   }

   template< typename Data >
   [[nodiscard]] constexpr bool ranges_overlap( const Data ll, const Data lh, const Data rl, const Data rh ) noexcept
   {
      // assert( ll <= lh );
      // assert( rl <= rh );
      return ( lh >= rl ) && ( rh >= ll );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
