// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REP_HH
#define PEGTL_INTERNAL_REP_HH

#include "skip_control.hh"
#include "trivial.hh"
#include "rule_conjunction.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Num, typename ... Rules > struct rep;

      template< unsigned Num, typename ... Rules >
      struct skip_control< rep< Num, Rules ... > > : std::true_type {};

      template< unsigned Num >
      struct rep< Num >
            : trivial< true > {};

      template< typename Rule, typename ... Rules >
      struct rep< 0, Rule, Rules ... >
            : trivial< true > {};

      template< unsigned Num, typename ... Rules >
      struct rep
      {
         using analyze_t = analysis::counted< analysis::rule_type::SEQ, Num, Rules ... >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< M >();

            for ( unsigned i = 0; i != Num; ++i ) {
               if ( ! rule_conjunction< Rules ... >::template match< A, rewind_mode::DONTCARE, Action, Control >( in, st ... ) ) {
                  return false;
               }
            }
            return m( true );
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
