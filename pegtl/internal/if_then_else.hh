// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_THEN_ELSE_HH
#define PEGTL_INTERNAL_IF_THEN_ELSE_HH

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Cond, typename Then, typename Else >
      struct if_then_else
      {
         using analyze_t = analysis::generic< analysis::rule_type::IF, Cond, Then, Else >;

         template< apply_mode A, error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();
            if ( Control< Cond >::template match< A, error_mode::RETURN, Action, Control >( in, st ... ) ) {
               return m( Control< Then >::template match< A, E, Action, Control >( in, st ... ) );
            }
            else {
               return m( Control< Else >::template match< A, E, Action, Control >( in, st ... ) );
            }
         }
      };

   } // internal

} // pegtl

#endif
