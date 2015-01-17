// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_MATCH_IMPL_HH
#define PEGTL_INTERNAL_RULE_MATCH_IMPL_HH

#include <cstdlib>

#include "apply_here.hh"
#include "rule_match_call.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule, error_mode E, template< typename ... > class Action, template< typename ... > class Control, apply_here H > struct rule_match_impl;

      template< typename Rule, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_impl< Rule, error_mode::RETURN, Action, Control, apply_here::NOTHING >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            Control< Rule >::start( static_cast< const Input & >( in ), st ... );

            if ( rule_match_call< Rule, error_mode::RETURN, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( static_cast< const Input & >( in ), st ... );
               return true;
            }
            Control< Rule >::failure( static_cast< const Input & >( in ), st ... );
            return false;
         }
      };

      template< typename Rule, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_impl< Rule, error_mode::THROW, Action, Control, apply_here::NOTHING >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            Control< Rule >::start( static_cast< const Input & >( in ), st ... );

            if ( rule_match_call< Rule, error_mode::THROW, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( static_cast< const Input & >( in ), st ... );
               return true;
            }
            Control< Rule >::raise( static_cast< const Input & >( in ), st ... );
            std::abort();
         }
      };

      template< typename Rule, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_impl< Rule, error_mode::RETURN, Action, Control, apply_here::ACTION >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.mark();

            Control< Rule >::start( static_cast< const Input & >( in ), st ... );

            if ( rule_match_call< Rule, error_mode::RETURN, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( static_cast< const Input & >( in ), st ... );
               Action< Rule >::apply( Input( m ), st ... );
               return m( true );
            }
            Control< Rule >::failure( static_cast< const Input & >( in ), st ... );
            return false;
         }
      };

      template< typename Rule, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_impl< Rule, error_mode::THROW, Action, Control, apply_here::ACTION >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.mark();

            Control< Rule >::start( static_cast< const Input & >( in ), st ... );

            if ( rule_match_call< Rule, error_mode::THROW, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( static_cast< const Input & >( in ), st ... );
               Action< Rule >::apply( Input( m ), st ... );
               return m( true );
            }
            Control< Rule >::raise( static_cast< const Input & >( in ), st ... );
            std::abort();
         }
      };

   } // internal

} // pegtl

#endif
