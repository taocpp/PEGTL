// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SOR_HH
#define PEGTL_INTERNAL_SOR_HH

#include "trivial.hh"
#include "rule_disjunction_impl.hh"

#include "../analysis/rule_class.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... > struct sor;

      template<>
      struct sor<>
            : trivial< false > {};

      template< typename Rule, typename ... Rules >
      struct sor< Rule, Rules ... >
      {
         using analyze_t = analysis::disjunction< Rule, Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_disjunction_impl< Rule, Rules ... >::template match< E, Action, Control >( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
