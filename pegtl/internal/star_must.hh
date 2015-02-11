// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_STAR_MUST_HH
#define PEGTL_INTERNAL_STAR_MUST_HH

#include "if_must.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct star_must
      {
         using analyze_t = analysis::generic< analysis::rule_type::STAR, Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            while ( ( ! in.empty() ) && rule_match_three< if_must< Rules ... >, error_mode::RETURN, Action, Control >::match( in, st ... ) ) {}
            return true;
         }
      };

   } // internal

} // pegtl

#endif
