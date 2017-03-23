// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_ONE_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_ONE_HH

#include "../config.hh"
#include "../apply_here.hh"
#include "../apply_mode.hh"
#include "../rewind_mode.hh"
#include "../control_mode.hh"

#include "rule_match_two.hh"
#include "rule_match_three.hh"
#include "use_no_data_apply.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      // Forward to rule_match_two<> for all user-defined / -visible rules,
      // but skip rule_match_two<> (which calls the control class' callbacks)
      // and forward directly to rule_match_three<> when a rule is marked as
      // an internal rule by skip_control<>.

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename ... > class Action,
                template< typename ... > class Control,
                control_mode C,
                typename ... States >
      struct rule_match_one;

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename ... States >
      struct rule_match_one< Rule, A, M, Action, Control, control_mode::NOTHING, States ... >
            : rule_match_three< Rule, A, M, Action, Control >
      { };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename ... States >
      struct rule_match_one< Rule, A, M, Action, Control, control_mode::CONTROL, States ... >
            : rule_match_two< Rule, A, M, Action, Control, ( ( A == apply_mode::NOTHING ) || is_nothing< Action, Rule >::value ) ? apply_here::NOTHING : ( use_no_data_apply< Action< Rule >, States ... >::value ? apply_here::ACTION_NO_DATA : apply_here::ACTION_WITH_DATA ) >
      { };

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
