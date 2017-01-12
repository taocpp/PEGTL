// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_APPLY_HH
#define PEGTL_INTERNAL_APPLY_HH

#include "trivial.hh"
#include "skip_control.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< apply_mode A, typename ... Actions > struct apply_impl;

      template< typename ... Actions >
      struct apply_impl< apply_mode::ACTION, Actions ... >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            using action_t = typename Input::action_t;
            const action_t i2( in.byte(), in.line(), in.byte_in_line(), in.begin(), in.begin(), in.source() );  // No data -- range is from begin to begin.
            using swallow = bool[];
            (void)swallow{ ( Actions::apply( i2, st ... ), true ) ..., true };
            return true;
         }
      };

      template< typename ... Actions >
      struct apply_impl< apply_mode::NOTHING, Actions ... >
      {
         template< typename Input, typename ... States >
         static bool match( Input &, States && ... )
         {
            return true;
         }
      };

      template< typename ... Actions >
      struct apply
      {
         using analyze_t = analysis::counted< analysis::rule_type::ANY, 0 >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return apply_impl< A, Actions ... >::match( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
