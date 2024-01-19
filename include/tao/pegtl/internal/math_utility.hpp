// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_UTILITY_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   [[nodiscard]] constexpr bool ranges_disjoint( const Data ll, const Data lh, const Data rl, const Data rh ) noexcept
   {
      // assert( ll < lh );
      // assert( rl < rh );
      return ( lh < rl ) || ( rh < ll );
   }

   template< typename Data >
   [[nodiscard]] constexpr bool ranges_overlap( const Data ll, const Data lh, const Data rl, const Data rh ) noexcept
   {
      // assert( ll < lh );
      // assert( rl < rh );
      return ( lh >= rl ) && ( rh >= ll );
   }

   [[nodiscard]] constexpr bool is_integer_fraction( const std::size_t n, const std::size_t d ) noexcept
   {
      return ( n / d ) * d == n;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
