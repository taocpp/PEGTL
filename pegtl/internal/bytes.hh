// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_BYTES_HH
#define PEGTL_INTERNAL_BYTES_HH

#include "skip_control.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Num >
      struct bytes
      {
         using analyze_t = analysis::counted< analysis::rule_type::ANY, Num >;

         // suppress warning with GCC 4.7
         template< typename T1, typename T2 >
         static inline bool dummy_greater_or_equal( const T1 a, const T2 b )
         {
            return a >= b;
         }

         template< typename Input >
         static bool match( Input & in )
         {
            if ( dummy_greater_or_equal( in.size( Num ), Num ) ) {
               in.bump( Num );
               return true;
            }
            return false;
         }
      };

      template< unsigned Num >
      struct skip_control< bytes< Num > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
