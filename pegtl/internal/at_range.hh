// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_AT_RANGE_HH
#define PEGTL_INTERNAL_AT_RANGE_HH

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< bool Want, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct at_range
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPTIONAL >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( in.size() ) {
               const auto t = Peek::peek( in );
               return t && ( Want == ( ( Lo <= t.data ) && ( t.data <= Hi ) ) );
            }
            return false;
         }
      };

   } // internal

} // pegtl

#endif
