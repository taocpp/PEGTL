// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RANGES_NOT_RANGES_HPP
#define TAO_PEGTL_INTERNAL_RANGES_NOT_RANGES_HPP

#include <cstddef>
#include <utility>

#include "../config.hpp"

#include "match_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< match_mode I, typename Peek, typename Peek::data_t... Cs >
   struct ranges_not_ranges
   {
      static_assert( sizeof...( Cs ) > 2 );

      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         using data_t = typename Peek::data_t;
         static_assert( sizeof( Data ) <= sizeof( data_t ) );
         return test_impl( std::make_index_sequence< sizeof...( Cs ) / 2 >(), data_t( c ) ) != bool( I );
      }

   private:
      template< typename Data, std::size_t... Is >
      [[nodiscard]] static constexpr bool test_impl( std::index_sequence< Is... > /*unused*/, const Data c ) noexcept
      {
         constexpr Data cs[] = { Cs... };

         if constexpr( sizeof...( Cs ) % 2 == 0 ) {
            return ( test_impl< Data, cs[ 2 * Is ], cs[ 2 * Is + 1 ] >( c ) || ... );
         }
         else {
            return ( test_impl< Data, cs[ 2 * Is ], cs[ 2 * Is + 1 ] >( c ) || ... ) || ( c == cs[ sizeof...( Cs ) - 1 ] );
         }
      }

      template< typename Data, Data Lo, Data Hi >
      [[nodiscard]] static constexpr bool test_impl( const Data c ) noexcept
      {
         static_assert( Lo <= Hi );
         return ( Lo <= c ) && ( c <= Hi );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
