// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_MODE_ELSE_HH
#define PEGTL_INTERNAL_IF_MODE_ELSE_HH

#include "utility.hh"
#include "rule_match_help.hh"

namespace pegtl
{
   namespace internal
   {
      template< error_mode Must, typename Cond, typename Then, typename Else >
      struct if_mode_else
      {
         using analyze_t = if_mode_else;

         template< error_mode Mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< merge( Must, Mode ) >();

            if ( rule_match_help< Cond, error_mode::RETURN, Action, Control >( in, st ... ) ) {
               return m( rule_match_help< Then, merge( Must, Mode ), Action, Control >( in, st ... ) );
            }
            else {
               return m( rule_match_help< Else, merge( Must, Mode ), Action, Control >( in, st ... ) );
            }
         }
      };

   } // internal

} // pegtl

#endif
