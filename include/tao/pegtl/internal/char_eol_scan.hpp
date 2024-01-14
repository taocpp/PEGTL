// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHAR_EOL_SCAN_HPP
#define TAO_PEGTL_INTERNAL_CHAR_EOL_SCAN_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char Eol, typename Position, typename Data >
   void char_eol_scan( Position& pos, const Data* data, const Data* dend ) noexcept
   {
      pos.count += dend - data;

      while( data < dend ) {
         if( *data++ == Eol ) {
            ++pos.line;
            pos.column = 1;
         }
         else {
            ++pos.column;
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
