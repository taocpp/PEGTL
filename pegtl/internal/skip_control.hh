// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SKIP_CONTROL_HH
#define PEGTL_INTERNAL_SKIP_CONTROL_HH

#include "rule_match_three.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule >
      struct skip_control
      {
         using analyze_t = typename Rule::analyze_t;

         template< apply_mode A, error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_three< Rule, A, E, Action, Control >::match( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
