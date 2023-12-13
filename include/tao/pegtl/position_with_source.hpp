// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_POSITION_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_POSITION_WITH_SOURCE_HPP

#include <ostream>
#include <utility>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Source, typename Position >
   struct position_with_source
      : Position
   {
      Source source;

      template< typename S, typename... Ps >
      explicit position_with_source( S&& s, Ps&&... ps )
         : Position( std::forward< Ps >( ps )... ),
           source( std::forward< S >( s ) )
      {}

      [[nodiscard]] Position& base() noexcept
      {
         return *this;
      }

      [[nodiscard]] const Position& base() const noexcept
      {
         return *this;
      }
   };

   template< typename Source, typename Position >
   [[nodiscard]] bool operator==( const position_with_source< Source, Position >& l, const position_with_source< Source, Position >& r ) noexcept
   {
      return ( l.source == r.source ) && ( l.base() == r.base() );
   }

   template< typename Source, typename Position >
   [[nodiscard]] bool operator!=( const position_with_source< Source, Position >& l, const position_with_source< Source, Position >& r ) noexcept
   {
      return !( l == r );
   }

   template< typename Source, typename Position >
   std::ostream& operator<<( std::ostream& os, const position_with_source< Source, Position >& p )
   {
      return os << p.source << '@' << p.base();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
