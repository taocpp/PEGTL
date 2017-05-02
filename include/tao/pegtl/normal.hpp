// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_NORMAL_HPP
#define TAOCPP_PEGTL_INCLUDE_NORMAL_HPP

#include "apply_mode.hpp"
#include "config.hpp"
#include "nothing.hpp"
#include "parse_error.hpp"
#include "rewind_mode.hpp"

#include "internal/demangle.hpp"
#include "internal/dusel_mode.hpp"
#include "internal/duseltronik.hpp"
#include "internal/has_apply0.hpp"
#include "internal/skip_control.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule >
      struct normal
      {
         template< typename Input, typename... States >
         static void start( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void success( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void failure( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void raise( const Input& in, States&&... )
         {
            throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
         }

         template< template< typename... > class Action, typename Input, typename... States >
         static void apply0( const Input&, States&&... st )
         {
            Action< Rule >::apply0( st... );
         }

         template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
         static void apply( const Iterator& begin, const Input& in, States&&... st )
         {
            using action_t = typename Input::action_t;
            const action_t action_input( begin, in );
            Action< Rule >::apply( action_input, st... );
         }

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename Input,
                   typename... States >
         static bool match( Input& in, States&&... st )
         {
            constexpr bool use_control = !internal::skip_control< Rule >::value;
            constexpr bool use_action = use_control && ( A == apply_mode::ACTION ) && ( !is_nothing< Action, Rule >::value );
            constexpr bool use_apply0 = use_action && internal::has_apply0< Action< Rule >, internal::type_list< States... > >::value;
            constexpr dusel_mode mode = static_cast< dusel_mode >( static_cast< char >( use_control ) + static_cast< char >( use_action ) + static_cast< char >( use_apply0 ) );
            return internal::duseltronik< Rule, A, M, Action, Control, mode >::match( in, st... );
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
