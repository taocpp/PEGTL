// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_TRIVIAL_HH
#define PEGTL_INTERNAL_TRIVIAL_HH

#include "skip_control.hh"
#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< bool What >
      struct trivial
      {
         using analyze_t = analysis::counted< analysis::rule_type::ANY, unsigned( ! What ) >;

         template< typename Input >
         static bool match( Input & )
         {
            return What;
         }
      };

      template< bool What >
      struct skip_control< trivial< What > > : std::true_type {};

   } // internal

} // pegtl

#endif
