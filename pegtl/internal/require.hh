// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REQUIRE_HH
#define PEGTL_INTERNAL_REQUIRE_HH

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Amount >
      struct require
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT >;

         template< typename Input >
         static bool match( Input & in )
         {
            return in.size( Amount ) >= Amount;
         }
      };

      template< unsigned Amount >
      struct skip_control< require< Amount > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
