// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DISCARD_HH
#define PEGTL_INTERNAL_DISCARD_HH

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      struct discard
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT >;

         template< typename Input >
         static bool match( Input & in )
         {
            in.discard();
            return true;
         }
      };

      template<>
      struct skip_control< discard > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
