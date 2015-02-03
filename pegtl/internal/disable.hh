// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DISABLE_HH
#define PEGTL_INTERNAL_DISABLE_HH

#include "../nothing.hh"

#include "seq.hh"

#include "../analysis/rule_class.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct disable
      {
         using analyze_t = analysis::conjunction< Rules ... >;

         template< error_mode E, template< typename ... > class, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_call< seq< Rules ... >, E, nothing, Control >::match( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
