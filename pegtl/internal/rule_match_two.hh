// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_MATCH_TWO_HH
#define PEGTL_INTERNAL_RULE_MATCH_TWO_HH

#include "../nothing.hh"
#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "rule_match_three.hh"

namespace pegtl
{
   namespace internal
   {
      // The purpose of rule_match_two is to call all necessary debug hooks of
      // the control class and, if applicable, also call the action class'
      // apply()-method. The latter can be disabled either explicitly (via
      // disable<>) or implicitly by at<> or not_at<>.

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename ... > class Action,
                template< typename ... > class Control,
                bool apply_here = ( ( A == apply_mode::ACTION ) && ( ! is_nothing< Action, Rule >::value ) ) >
      struct rule_match_two;

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_two< Rule, A, M, Action, Control, false >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            Control< Rule >::start( const_cast< const Input & >( in ), st ... );

            if ( rule_match_three< Rule, A, M, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( const_cast< const Input & >( in ), st ... );
               return true;
            }
            Control< Rule >::failure( const_cast< const Input & >( in ), st ... );
            return false;
         }
      };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_two< Rule, A, M, Action, Control, true >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< rewind_mode::REQUIRED >();  // TODO: Allow actions to opt-out of receiving input data?

            using action_t = typename Input::action_t;

            if ( rule_match_two< Rule, A, M, Action, Control, false >::match( in, st ... ) ) {
               Action< Rule >::apply( action_t( m, in.data() ), st ... );
               return m( true );
            }
            return false;
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
