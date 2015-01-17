// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_DISJUNCTION_IMPL_HH
#define PEGTL_INTERNAL_RULE_DISJUNCTION_IMPL_HH

#include "rule_match_help.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... > struct rule_disjunction_impl;

      template<>
      struct rule_disjunction_impl<>
      {
         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input &, States && ... )
         {
            return false;
         }
      };

      template< typename Rule, typename ... Rules >
      struct rule_disjunction_impl< Rule, Rules ... >
      {
         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_help< Rule, error_mode::RETURN, Action, Control >( in, st ... ) || rule_disjunction_impl< Rules ... >::template match< error_mode::RETURN, Action, Control >( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
