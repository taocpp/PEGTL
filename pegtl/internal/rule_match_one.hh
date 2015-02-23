// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_MATCH_ONE_HH
#define PEGTL_INTERNAL_RULE_MATCH_ONE_HH

#include <type_traits>

#include "../nothing.hh"

#include "../apply_mode.hh"

#include "utility.hh"

#include "skip_control.hh"
#include "rule_match_two.hh"
#include "rule_match_three.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule, apply_mode A, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_two< Rule, A, Action, Control, std::is_base_of< nothing< Rule >, Action< Rule > >::value ? merge( A, apply_here::NOTHING ) : merge( A, apply_here::ACTION ) >::match( in, st ... );
         }
      };

      template< typename Rule, apply_mode A, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one< skip_control< Rule >, A, Action, Control >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            return rule_match_three< Rule, A, Action, Control >::match( in, st ... );
         }
      };

   } // internal

} // pegtl

#endif
