// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_PLUS_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_PLUS_HH

#include <type_traits>

#include "../config.hh"

#include "opt.hh"
#include "duseltronik.hh"
#include "seq.hh"
#include "star.hh"
#include "skip_control.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      // While plus<> could easily be implemented with
      // seq< Rule, Rules ..., star< Rule, Rules ... > > we
      // provide an explicit implementation to optimize away
      // the otherwise created input mark.

      template< typename Rule, typename ... Rules >
      struct plus
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, Rule, Rules ..., opt< plus > >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return duseltronik< seq< Rule, Rules ... >, A, M, Action, Control >::match( in, st ... ) && duseltronik< star< Rule, Rules ... >, A, M, Action, Control >::match( in, st ... );
         }
      };

      template< typename Rule, typename ... Rules >
      struct skip_control< plus< Rule, Rules ... > > : std::true_type {};

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
