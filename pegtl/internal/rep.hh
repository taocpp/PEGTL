// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_REP_HH
#define PEGTL_INTERNAL_REP_HH

#include "trivial.hh"
#include "rule_conjunction_impl.hh"

namespace pegtl
{
   namespace internal
   {
      template< unsigned, typename ... > struct rep;

      template< unsigned Num >
      struct rep< Num >
            : trivial< true > {};

      template< typename Rule, typename ... Rules >
      struct rep< 0, Rule, Rules ... >
            : trivial< true > {};

      template< unsigned Num, typename Rule, typename ... Rules >
      struct rep< Num, Rule, Rules ... >
      {
         using internal_t = rep;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();

            for ( unsigned i = 0; i < Num; ++i ) {
               if ( ! rule_conjunction_impl< Rule, Rules ... >::template match< E, Action, Control >( in, st ... ) ) {
                  return m( false );
               }
            }
            return m( true );
         }
      };

   } // internal

} // pegtl

#endif
