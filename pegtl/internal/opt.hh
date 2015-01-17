// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_OPT_HH
#define PEGTL_INTERNAL_OPT_HH

#include "seq.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct opt
      {
         using internal_t = opt;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return in.empty() || rule_match_call< seq< Rules ... >, error_mode::RETURN, Action, Control >::match( in, st ... ) || true;
         }
      };

   } // internal

} // pegtl

#endif
