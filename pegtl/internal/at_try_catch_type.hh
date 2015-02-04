// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_AT_TRY_CATCH_TYPE_HH
#define PEGTL_INTERNAL_AT_TRY_CATCH_TYPE_HH

#include "../nothing.hh"

#include "at.hh"
#include "seq.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   struct parse_error;

   namespace internal
   {
      template< typename Exception, typename ... Rules > struct at_try_catch_type;

      template< typename Exception >
      struct at_try_catch_type< Exception >
            : trivial< true > {};

      template< typename Exception, typename Rule, typename ... Rules >
      struct at_try_catch_type< Exception, Rule, Rules ... >
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPTIONAL, Rule, Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.mark();

            try {
               return rule_match_call< seq< Rule, Rules ... >, E, nothing, Control >::match( in, st ... );
            }
            catch ( const Exception & ) {
               return false;
            }
         }
      };

   } // internal

} // pegtl

#endif
