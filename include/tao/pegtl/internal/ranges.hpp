// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RANGES_HPP
#define TAO_PEGTL_INTERNAL_RANGES_HPP

#include <utility>

#include "../config.hpp"

#include "enable_control.hpp"
#include "failure.hpp"
#include "one.hpp"
#include "range.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct ranges
      : single< ranges< Peek, Cs... > >
   {
      template< typename Data, Data Lo, Data Hi >
      [[nodiscard]] static constexpr bool test_two( const Data c ) noexcept
      {
         static_assert( Lo <= Hi );
         return ( Lo <= c ) && ( c <= Hi );
      }

      template< typename Data, std::size_t... Is >
      [[nodiscard]] static constexpr bool test_one( std::index_sequence< Is... > /*unused*/, const Data c ) noexcept
      {
         constexpr Data cs[] = { Cs... };

         if constexpr( sizeof...( Cs ) % 2 == 0 ) {
            return ( test_two< Data, cs[ 2 * Is ], cs[ 2 * Is + 1 ] >( c ) || ... );
         }
         else {
            return ( test_two< Data, cs[ 2 * Is ], cs[ 2 * Is + 1 ] >( c ) || ... ) || ( c == cs[ sizeof...( Cs ) - 1 ] );
         }
      }

      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return test_one( std::make_index_sequence< sizeof...( Cs ) / 2 >(), c );
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct ranges< Peek, Lo, Hi >
      : range< Peek, Lo, Hi >
   {};

   template< typename Peek, typename Peek::data_t C >
   struct ranges< Peek, C >
      : one< Peek, C >
   {};

   template< typename Peek >
   struct ranges< Peek >
      : failure
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< ranges< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
