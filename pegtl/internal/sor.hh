// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SOR_HH
#define PEGTL_INTERNAL_SOR_HH

#include "trivial.hh"
#include "rule_disjunction.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules > struct sor;

      template<>
      struct sor<>
            : trivial< false > {};

      template< typename Rule >
      struct sor< Rule >
      {
         using analyze_t = typename Rule::analyze_t;

         template< apply_mode A, error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return Control< Rule >::template match< A, E, Action, Control >( in, st ... );
         }
      };

      template< typename Rule, typename More, typename ... Rules >
      struct sor< Rule, More, Rules ... >
      {
         using analyze_t = analysis::generic< analysis::rule_type::SOR, Rule, More, Rules ... >;

         template< apply_mode A, error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_disjunction< Rule, More, Rules ... >::template match< A, E, Action, Control >( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
