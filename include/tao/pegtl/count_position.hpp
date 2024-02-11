// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_COUNT_POSITION_HPP
#define TAO_PEGTL_COUNT_POSITION_HPP

#include <cstddef>
#include <ostream>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct count_position
   {
      std::size_t count = 0;

      count_position() noexcept = default;

      explicit count_position( const std::size_t in_count ) noexcept
         : count( in_count )
      {}
   };

   inline std::ostream& operator<<( std::ostream& os, const count_position p )
   {
      return os << p.count;
   }

   [[nodiscard]] inline bool operator==( const count_position l, const count_position r ) noexcept
   {
      return l.count == r.count;
   }

   [[nodiscard]] inline bool operator!=( const count_position l, const count_position r ) noexcept
   {
      return !( l == r );
   }

   inline count_position& operator+= ( count_position& l, const count_position r ) noexcept
   {
      l.count += r.count;
      return l;
   }

   [[nodiscard]] inline count_position operator+( const count_position l, const count_position r ) noexcept
   {
      return count_position( l.count + r.count );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
