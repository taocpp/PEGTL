// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_EXTRACT_POSITION_HPP
#define TAO_PEGTL_INTERNAL_EXTRACT_POSITION_HPP

#include "../config.hpp"

#include "has_current_position.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   [[nodiscard]] decltype( auto ) extract_position( const T& t )
   {
      if constexpr( !has_current_position< T > ) {
         return t;  // Assume t is not an input.
      }
      else if constexpr( !has_current_position< decltype( *t.current() ) > ) {
         return t.current_position();  // Assume t is an input of "simple" object (char, enums).
      }
      else {
         return t.current()->current_position();  // Assume t is an input of objects that already have a position (tokens).
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
