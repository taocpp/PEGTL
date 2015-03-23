// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RANGE_HH
#define PEGTL_INTERNAL_RANGE_HH

#include "skip_control.hh"
#include "any.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< bool Want, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct range
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( Want == ( ( Lo <= t.data ) && ( t.data <= Hi ) ) ) {
                     in.bump( t.size );
                     return true;
                  }
               }
            }
            return false;
         }
      };

      template< bool Want, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct skip_control< range< Want, Peek, Lo, Hi > > : std::true_type {};

   } // internal

} // pegtl

#endif
