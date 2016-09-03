// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_PEEK_UTF32_HH
#define PEGTL_INTERNAL_PEEK_UTF32_HH

#include <cstddef>

#include "input_pair.hh"

namespace pegtl
{
   namespace internal
   {
      struct peek_utf32
      {
         using data_t = char32_t;
         using pair_t = input_pair< char32_t >;

         static_assert( sizeof( char32_t ) == 4, "expected size 4 for 32bit value" );

         // suppress warning with GCC 4.7
         template< typename T >
         static inline bool dummy_less_or_equal( const T a, const T b )
         {
            return a <= b;
         }

         template< typename Input >
         static pair_t peek( Input & in )
         {
            const std::size_t s = in.size( 4 );
            if ( s >= 4 ) {
               const char32_t t = * reinterpret_cast< const char32_t * >( in.begin() );
               if ( dummy_less_or_equal< char32_t >( 0, t ) && dummy_less_or_equal< char32_t >( t, 0x10ffff ) ) {
                  return { t, 4 };
               }
            }
            return { 0, 0 };
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
