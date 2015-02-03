// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_MODE_HH
#define PEGTL_INTERNAL_IF_MODE_HH

#include "utility.hh"
#include "rule_match_help.hh"
#include "rule_conjunction_impl.hh"

namespace pegtl
{
   namespace internal
   {
      template< error_mode Must, typename Cond, typename ... Thens >
      struct if_mode
      {
         using analyze_t = if_mode;

         template< error_mode Mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< merge( Must, Mode ) >();

            if ( rule_match_help< Cond, error_mode::RETURN, Action, Control >( in, st ... ) ) {
               return m( rule_conjunction_impl< Thens ... >::template match< merge( Must, Mode ), Action, Control >( in, st ... ) );
            }
            return ! bool( Must );
         }
      };

   } // internal

} // pegtl

#endif
