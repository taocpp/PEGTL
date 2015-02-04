// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REP_MIN_MAX_HH
#define PEGTL_INTERNAL_REP_MIN_MAX_HH

#include "seq.hh"
#include "not_at.hh"
#include "trivial.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Min, unsigned Max, typename ... Rules >
      struct rep_min_max
      {
         using analyze_t = analysis::counted< analysis::rule_type::CONJUNCTION, Min, Rules ... >;

         static_assert( Min <= Max, "pegtl: illegal rep_min_max rule (maximum number of repetitions smaller than minimum)" );

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();

            for ( unsigned i = 0; i < Min; ++i ) {
               if ( ! rule_conjunction_impl< Rules ... >::template match< E, Action, Control >( in, st ... ) ) {
                  return m( false );
               }
            }
            for ( unsigned i = Min; i < Max; ++i ) {
               if ( ! rule_match_call< seq< Rules ... >, error_mode::RETURN, Action, Control >::match( in, st ... ) ) {
                  return m( true );
               }
            }
            return m( rule_match_call< not_at< Rules ... >, E, Action, Control >::match( in, st ... ) );
         }
      };

   } // internal

} // pegtl

#endif
