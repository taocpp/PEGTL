// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_MATCH_CALL_HH
#define PEGTL_INTERNAL_RULE_MATCH_CALL_HH

#include "../error_mode.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule, error_mode E, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_call
      {
         template< typename Input, typename ... States >
         static auto match( Input & in, States && ... st ) -> decltype( Rule::template match< E, Action, Control >( in, st ... ), bool() )
         {
            return Rule::template match< E, Action, Control >( in, st ... );
         }

         // Note: The additional int=0 is a work-around for VS2015
         template< typename Input, typename ... States, int = 0 >
         static auto match( Input & in, States && ... ) -> decltype( Rule::match( in ), bool() )
         {
            return Rule::match( in );
         }
      };

   } // internal

} // pegtl

#endif
