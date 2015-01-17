// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_UNTIL_HH
#define PEGTL_INTERNAL_UNTIL_HH

#include "rule_match_help.hh"
#include "rule_conjunction_impl.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Cond, typename ... Rules > struct until;

      template< typename Cond >
      struct until< Cond >
      {
         using internal_t = until;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();

            while ( ! rule_match_help< Cond, error_mode::RETURN, Action, Control >( in, st ... ) ) {
               if ( ! in.bump_if() ) {
                  return false;
               }
            }
            m.success();
            return true;
         }
      };

      template< typename Cond, typename Rule, typename ... Rules >
      struct until< Cond, Rule, Rules ... >
      {
         using internal_t = until;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();

            while ( ! rule_match_help< Cond, error_mode::RETURN, Action, Control >( in, st ... ) ) {
               if ( in.empty() || ! rule_conjunction_impl< Rule, Rules ... >::template match< E, Action, Control >( in, st ... ) ) {
                  return false;
               }
            }
            m.success();
            return true;
         }
      };

   } // internal

} // pegtl

#endif
