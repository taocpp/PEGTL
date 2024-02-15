// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_POSITION_HPP
#define TAO_PEGTL_TEXT_POSITION_HPP

#include <cstddef>
#include <ostream>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct text_position
   {
      std::size_t line = 1;
      std::size_t column = 1;
      std::size_t count = 0;

      text_position() = default;

      text_position( const std::size_t in_line, const std::size_t in_column, const std::size_t in_count ) noexcept
         : line( in_line ),
           column( in_column ),
           count( in_count )
      {}

      void apply_offset( const text_position& offset ) noexcept
      {
         // assert( ... )

         if( line == 1 ) {
            line = offset.line;
            column += offset.column - 1;
         }
         else {
            line += offset.line - 1;
         }
         count += offset.count;
      }
   };

   inline std::ostream& operator<<( std::ostream& os, const text_position p )
   {
      return os << p.line << ':' << p.column << '(' << p.count << ')';
   }

   [[nodiscard]] inline bool operator==( const text_position l, const text_position r ) noexcept
   {
      return ( l.count == r.count ) && ( l.line == r.line ) && ( l.column == r.column );
   }

   [[nodiscard]] inline bool operator!=( const text_position l, const text_position r ) noexcept
   {
      return !( l == r );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
