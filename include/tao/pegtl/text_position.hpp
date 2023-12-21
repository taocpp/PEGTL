// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_POSITION_HPP
#define TAO_PEGTL_TEXT_POSITION_HPP

#include <cstddef>
#include <ostream>
#include <type_traits>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct text_position
   {
      std::size_t count = 0;
      std::size_t line = 1;
      std::size_t column = 1;

      text_position() = default;

      text_position( const std::size_t in_count, const std::size_t in_line, const std::size_t in_column ) noexcept
         : count( in_count ),
           line( in_line ),
           column( in_column )
      {}
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

   inline text_position& operator+=( text_position& l, const text_position& r ) noexcept
   {
      if( r.line == 1 ) {
         l.column += r.column - 1;
      }
      else {
         l.line += r.line - 1;
         l.column = r.column;
      }
      l.count += r.count;
      return l;
   }

   [[nodiscard]] inline text_position operator+( const text_position& l, const text_position& r ) noexcept
   {
      text_position t( l );
      t += r;
      return t;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
