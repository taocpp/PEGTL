// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_PLUS_HH
#define PEGTL_INTERNAL_PLUS_HH

#include "seq.hh"
#include "star.hh"

#include "../analysis/rule_class.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct plus
      {
         using analyze_t = analysis::conjunction< Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_call< seq< Rules ... >, E, Action, Control >::match( in, st ... ) && rule_match_call< star< Rules ... >, E, Action, Control >::match( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
