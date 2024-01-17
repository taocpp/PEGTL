// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHAR_EOL_SCAN_HPP
#define TAO_PEGTL_INTERNAL_CHAR_EOL_SCAN_HPP

#include <cstddef>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char Eol, typename Position, typename Data >
   void char_eol_scan( Position& pos, const Data* data, const std::size_t count ) noexcept
   {
      for( std::size_t i = 0; i < count; ++i ) {
         if( data[ i ] == Eol ) {
            ++pos.line;
            pos.column = 1;
         }
         else {
            ++pos.column;
         }
      }
      pos.count += count;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
