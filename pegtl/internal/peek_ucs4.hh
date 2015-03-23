// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_PEEK_UCS4_HH
#define PEGTL_INTERNAL_PEEK_UCS4_HH

#include "input_pair.hh"

namespace pegtl
{
   namespace internal
   {
      struct peek_ucs4
      {
         using data_t = char32_t;
         using pair_t = input_pair< char32_t >;

         template< typename Input >
         static pair_t peek( Input & in )
         {
            const std::size_t s = in.size();
            if ( s >= sizeof( char32_t ) ) {
               const char32_t t = * reinterpret_cast< const char32_t * >( in.begin() );
               if ( ( 0 <= t ) && ( t <= 0x10ffff ) ) {
                  return { t, 4 };
               }
            }
            return { 0, 0 };
         }
      };

   } // internal

} // pegtl

#endif
