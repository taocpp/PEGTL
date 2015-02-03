// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SEQ_HH
#define PEGTL_INTERNAL_SEQ_HH

#include "trivial.hh"
#include "rule_conjunction_impl.hh"

#include "../analysis/rule_class.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... > struct seq;

      template<>
      struct seq<>
            : trivial< true > {};

      template< typename Rule >
      struct seq< Rule >
      {
         using analyze_t = analysis::conjunction< Rule >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_help< Rule, E, Action, Control >( in, st ... );
         }
      };

      template< typename Rule, typename More, typename ... Rules >
      struct seq< Rule, More, Rules ... >
      {
         using analyze_t = analysis::conjunction< Rule, More, Rules ... >;

         template< error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< E >();
            return m( rule_conjunction_impl< Rule, More, Rules ... >::template match< E, Action, Control >( in, st ... ) );
         }
      };

   } // internal

} // pegtl

#endif
