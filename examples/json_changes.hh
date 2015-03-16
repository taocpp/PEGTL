// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_EXAMPLES_JSON_CHANGES_HH
#define PEGTL_EXAMPLES_JSON_CHANGES_HH

#include <pegtl/normal.hh>

namespace examples
{
   struct dummy_disabled_state
   {
      template< typename ... Ts >
      dummy_disabled_state( Ts && ... )
      { }

      template< typename ... Ts >
      void success( Ts && ... )
      { }
   };

   template< pegtl::apply_mode A, typename State > struct state_disable_helper;

   template< typename State > struct state_disable_helper< pegtl::apply_mode::ACTION, State > { using state_t = State; };
   template< typename State > struct state_disable_helper< pegtl::apply_mode::NOTHING, State > { using state_t = dummy_disabled_state; };

   template< typename Rule, typename State, template< typename ... > class Base = pegtl::normal >
   struct change_state
         : public Base< Rule >
   {
      template< pegtl::apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         typename state_disable_helper< A, State >::state_t s( static_cast< const Input & >( in ), st ... );

         if ( Base< Rule >::template match< A, Action, Control >( in, s ) ) {
            s.success( static_cast< const Input & >( in ), st ... );
            return true;
         }
         return false;
      }
   };

   template< typename Rule, template< typename ... > class Action, template< typename ... > class Base = pegtl::normal >
   struct change_action
         : public Base< Rule >
   {
      template< pegtl::apply_mode A, template< typename ... > class, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         return Base< Rule >::template match< A, Action, Control >( in, st ... );
      }
   };

   template< template< typename ... > class Action, template< typename ... > class Base >
   struct change_both_helper
   {
      template< typename T > using change_action = change_action< T, Action, Base >;
   };

   template< typename Rule, typename State, template< typename ... > class Action, template< typename ... > class Base = pegtl::normal >
   struct change_state_and_action
         : public change_state< Rule, State, change_both_helper< Action, Base >::template change_action >
   { };

} // examples

#endif
