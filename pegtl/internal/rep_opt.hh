// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REP_OPT_HH
#define PEGTL_INTERNAL_REP_OPT_HH

#include "seq.hh"
#include "rule_match_three.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned Max, typename ... Rules >
      struct rep_opt
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT, Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            for ( unsigned i = 0; ( i < Max ) && rule_match_three< seq< Rules ... >, error_mode::RETURN, Action, Control >::match( in, st ... ); ++i ) {}
            return true;
         }
      };

   } // internal

} // pegtl

#endif
