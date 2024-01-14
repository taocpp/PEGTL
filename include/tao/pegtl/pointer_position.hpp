// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_POINTER_POSITION_HPP
#define TAO_PEGTL_POINTER_POSITION_HPP

#include <ostream>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Data >
   struct pointer_position
   {
      const Data* data = nullptr;

      pointer_position() noexcept = default;

      explicit pointer_position( const Data* in_data ) noexcept
         : data( in_data )
      {}
   };

   template< typename Data >
   pointer_position( const Data* ) -> pointer_position< Data >;

   template< typename Data >
   [[nodiscard]] bool operator==( const pointer_position< Data > l, const pointer_position< Data > r ) noexcept
   {
      return l.current == r.current;
   }

   template< typename Data >
   [[nodiscard]] bool operator!=( const pointer_position< Data > l, const pointer_position< Data > r ) noexcept
   {
      return !( l == r );
   }

   template< typename Data >
   std::ostream& operator<<( std::ostream& os, const pointer_position< Data > p )
   {
      return os << static_cast< const volatile void* >( p.data );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
