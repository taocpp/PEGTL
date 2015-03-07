// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_CONJUNCTION_HH
#define PEGTL_INTERNAL_RULE_CONJUNCTION_HH

#include "../apply_mode.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... > struct rule_conjunction;

      template<>
      struct rule_conjunction<>
      {
         template< apply_mode, template< typename ... > class, template< typename ... > class, typename Input, typename ... States >
         static bool match( Input &, States && ... )
         {
            return true;
         }
      };

      template< typename Rule, typename ... Rules >
      struct rule_conjunction< Rule, Rules ... >
      {
         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return Control< Rule >::template match< A, Action, Control >( in, st ... ) && rule_conjunction< Rules ... >::template match< A, Action, Control >( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
