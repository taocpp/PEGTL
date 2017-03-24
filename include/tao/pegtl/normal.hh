// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_NORMAL_HH
#define TAOCPP_PEGTL_INCLUDE_NORMAL_HH

#include "config.hh"
#include "nothing.hh"
#include "count_data.hh"
#include "apply_mode.hh"
#include "rewind_mode.hh"
#include "parse_error.hh"

#include "internal/demangle.hh"
#include "internal/has_apply0.hh"
#include "internal/action_input.hh"
#include "internal/duseltronik.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct normal
   {
      template< typename Input, typename ... States >
      static void start( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void success( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void failure( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void raise( const Input & in, States && ... )
      {
         throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
      }

      template< template< typename ... > class Action, typename ... States >
      static void apply0( States && ... st )
      {
         Action< Rule >::apply0( st ... );
      }

      template< typename Input, template< typename ... > class Action, typename ... States >
      static void apply( const count_data & begin, const count_data & end, const char * source, States && ... st )
      {
         const Input in( begin, end.data, source );
         Action< Rule >::apply( in, st ... );
      }

      template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         constexpr bool use_control = ! internal::skip_control< Rule >::value;
         constexpr bool use_action = use_control && ( A == apply_mode::ACTION ) && ( ! is_nothing< Action, Rule >::value );
         constexpr bool use_apply0 = use_action && internal::has_apply0< Action< Rule >, internal::type_list< States ... > >::value;
         return internal::duseltronik< Rule, A, M, Action, Control, use_control, use_action, use_apply0 >::match( in, st ... );
      }
   };

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
