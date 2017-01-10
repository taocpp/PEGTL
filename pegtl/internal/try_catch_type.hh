// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_TRY_CATCH_TYPE_HH
#define PEGTL_INTERNAL_TRY_CATCH_TYPE_HH

#include <type_traits>

#include "skip_control.hh"
#include "trivial.hh"
#include "rule_match_three.hh"
#include "seq.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Exception, typename ... Rules > struct try_catch_type;

      template< typename Exception, typename ... Rules >
      struct skip_control< try_catch_type< Exception, Rules ... > > : std::true_type {};

      template< typename Exception >
      struct try_catch_type< Exception >
            : trivial< true > {};

      template< typename Exception, typename ... Rules >
      struct try_catch_type
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, Rules ... >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< M >();

            try {
               return m( rule_match_three< seq< Rules ... >, A, rewind_mode::DONTCARE, Action, Control >::match( in, st ... ) );
            }
            catch ( const Exception & ) {
               return false;
            }
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
