// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_STATE_HH
#define PEGTL_INTERNAL_STATE_HH

#include "seq.hh"
#include "rule_match_three.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename State, typename ... Rules >
      struct state
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, Rules ... >;

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            State s{ static_cast< const Input & >( in ), st ... };

            if ( rule_match_three< seq< Rules ... >, A, Action, Control >::match( in, s ) ) {
               s.success( static_cast< const Input & >( in ), st ... );
               return true;
            }
            return false;
         }
      };

   } // internal

} // pegtl

#endif
