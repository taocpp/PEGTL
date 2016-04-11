// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DISCARD_IF_HH
#define PEGTL_INTERNAL_DISCARD_IF_HH

#include "skip_control.hh"
#include "seq.hh"
#include "rule_match_three.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct discard_if
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, Rules ... >;

         template< apply_mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            if ( rule_match_three< seq< Rules ... >, apply_mode::ACTION, Action, Control >::match( in, st ... ) ) {
               in.discard();
               return true;
            }
            return false;
         }
      };

      template< typename ... Rules >
      struct skip_control< discard_if< Rules ... > > : std::true_type {};

   } // internal

} // pegtl

#endif
