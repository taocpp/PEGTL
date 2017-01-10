// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REP_MIN_MAX_HH
#define PEGTL_INTERNAL_REP_MIN_MAX_HH

#include <type_traits>

#include "skip_control.hh"
#include "trivial.hh"
#include "not_at.hh"
#include "rule_conjunction.hh"
#include "rule_match_three.hh"
#include "seq.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Min, unsigned Max, typename ... Rules > struct rep_min_max;

      template< unsigned Min, unsigned Max, typename ... Rules >
      struct skip_control< rep_min_max< Min, Max, Rules ... > > : std::true_type {};

      template< unsigned Min, unsigned Max >
      struct rep_min_max< Min, Max >
            : trivial< false >
      {
         static_assert( Min <= Max, "invalid rep_min_max rule (maximum number of repetitions smaller than minimum)" );
      };

      template< typename Rule, typename ... Rules >
      struct rep_min_max< 0, 0, Rule, Rules ... >
            : not_at< Rule, Rules ... >
      { };

      template< unsigned Min, unsigned Max, typename ... Rules >
      struct rep_min_max
      {
         using analyze_t = analysis::counted< analysis::rule_type::SEQ, Min, Rules ... >;

         static_assert( Min <= Max, "invalid rep_min_max rule (maximum number of repetitions smaller than minimum)" );

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< M >();

            for ( unsigned i = 0; i != Min; ++i ) {
               if ( ! rule_conjunction< Rules ... >::template match< A, rewind_mode::DONTCARE, Action, Control >( in, st ... ) ) {
                  return false;
               }
            }
            for ( unsigned i = Min; i != Max; ++i ) {
               if ( ! rule_match_three< seq< Rules ... >, A, rewind_mode::REQUIRED, Action, Control >::match( in, st ... ) ) {
                  return m( true );
               }
            }
            return m( rule_match_three< not_at< Rules ... >, A, M, Action, Control >::match( in, st ... ) );
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
