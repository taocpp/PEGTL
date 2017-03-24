// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_ONE_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_ONE_HH

#include "../config.hh"
#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "skip_control.hh"
#include "rule_match_three.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename ... > class Action,
                template< typename ... > class Control,
                bool Use_Control,
                bool Use_Action,
                bool Use_Apply0 >
      struct rule_match_one;

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one< Rule, A, M, Action, Control, false, false, false >
            : rule_match_three< Rule, A, M, Action, Control >
      { };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one< Rule, A, M, Action, Control, true, false, false >
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
      struct rule_match_one< Rule, A, M, Action, Control, true, false, true >
            : rule_match_one< Rule, A, M, Action, Control, true, false, false >
      { };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one< Rule, A, M, Action, Control, true, true, false >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< rewind_mode::REQUIRED >();

            if ( rule_match_one< Rule, A, rewind_mode::ACTIVE, Action, Control, true, false, false >::match( in, st ... ) ) {
               Control< Rule >::template apply< typename Input::action_t, Action >( m.count(), in.count(), in.source(), st ... );
               return m( true );
            }
            return false;
         }
      };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_one< Rule, A, M, Action, Control, true, true, true >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            if ( rule_match_one< Rule, A, rewind_mode::ACTIVE, Action, Control, true, false, false >::match( in, st ... ) ) {
               Control< Rule >::template apply0< Action >( st ... );
               return true;
            }
            return false;
         }
      };

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
