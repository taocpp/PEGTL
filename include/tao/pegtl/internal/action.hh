// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_ACTION_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_ACTION_HH

#include "../config.hh"

#include "skip_control.hh"
#include "seq.hh"
#include "duseltronik.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< template< typename ... > class Action, typename ... Rules >
         struct action
         {
            using analyze_t = analysis::generic< analysis::rule_type::SEQ, Rules ... >;

            template< apply_mode A, rewind_mode M, template< typename ... > class, template< typename ... > class Control, typename Input, typename ... States >
            static bool match( Input & in, States && ... st )
            {
               return duseltronik< seq< Rules ... >, A, M, Action, Control >::match( in, st ... );
            }
         };

         template< template< typename ... > class Action, typename ... Rules >
         struct skip_control< action< Action, Rules ... > > : std::true_type {};

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
