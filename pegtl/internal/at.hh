// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_AT_HH
#define PEGTL_INTERNAL_AT_HH

#include "skip_control.hh"
#include "trivial.hh"
#include "rule_conjunction.hh"

#include "../apply_mode.hh"
#include "../marker_mode.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules > struct at;

      template< typename ... Rules >
      struct skip_control< at< Rules ... > > : std::true_type {};

      template<>
      struct at<>
            : trivial< true > {};

      template< typename ... Rules >
      struct at
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT, Rules ... >;

         template< apply_mode, marker_mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            const auto m = in.template mark< marker_mode::ENABLED >();
            return rule_conjunction< Rules ... >::template match< apply_mode::NOTHING, marker_mode::DISABLED, Action, Control >( in, st ... );
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
