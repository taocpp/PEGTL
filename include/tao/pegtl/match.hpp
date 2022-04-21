// Copyright (c) 2019-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_MATCH_HPP
#define TAO_PEGTL_MATCH_HPP

#include <type_traits>

#include "apply_mode.hpp"
#include "nothing.hpp"
#include "require_apply.hpp"
#include "require_apply0.hpp"
#include "rewind_mode.hpp"

#include "internal/has_apply.hpp"
#include "internal/has_apply0.hpp"
#include "internal/has_unwind.hpp"
#include "internal/missing_apply.hpp"
#include "internal/missing_apply0.hpp"
#include "internal/rewind_guard.hpp"
#include "internal/unwind_guard.hpp"

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

namespace tao::pegtl
{
   namespace internal
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static auto match_no_control( ParseInput& in, States&&... st )
         -> decltype( Rule::template match< A, M, Action, Control >( in, st... ) )
      {
         return Rule::template match< A, M, Action, Control >( in, st... );
      }

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static auto match_no_control( ParseInput& in, States&&... /*unused*/ )
         -> decltype( Rule::match( in ) )
      {
         return Rule::match( in );
      }

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] auto match_control_unwind( ParseInput& in, States&&... st )
      {
#if defined( __cpp_exceptions )
         if constexpr( has_unwind< Control< Rule >, void, const ParseInput&, States... > ) {
            unwind_guard ug( [ & ]() {
               Control< Rule >::unwind( static_cast< const ParseInput& >( in ), st... );
            } );
            auto result = match_no_control< Rule, A, M, Action, Control >( in, st... );
            ug.unwind.reset();
            return result;
         }
         else {
            return match_no_control< Rule, A, M, Action, Control >( in, st... );
         }
#else
         return match_no_control< Rule, A, M, Action, Control >( in, st... );
#endif
      }

   }  // namespace internal

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control,
             typename ParseInput,
             typename... States >
   [[nodiscard]] auto match( ParseInput& in, States&&... st )
   {
      if constexpr( !Control< Rule >::enable ) {
         return internal::match_no_control< Rule, A, M, Action, Control >( in, st... );
      }
      else {
         constexpr bool enable_action = ( A == apply_mode::action );

         using frobnicator_t = typename ParseInput::frobnicator_t;
         constexpr bool has_apply_void = enable_action && internal::has_apply< Control< Rule >, void, Action, const frobnicator_t&, const ParseInput&, States... >;
         constexpr bool has_apply_bool = enable_action && internal::has_apply< Control< Rule >, bool, Action, const frobnicator_t&, const ParseInput&, States... >;
         constexpr bool has_apply = has_apply_void || has_apply_bool;

         constexpr bool has_apply0_void = enable_action && internal::has_apply0< Control< Rule >, void, Action, const ParseInput&, States... >;
         constexpr bool has_apply0_bool = enable_action && internal::has_apply0< Control< Rule >, bool, Action, const ParseInput&, States... >;
         constexpr bool has_apply0 = has_apply0_void || has_apply0_bool;

         static_assert( !( has_apply && has_apply0 ), "both apply() and apply0() defined" );

         constexpr bool is_nothing = std::is_base_of_v< nothing< Rule >, Action< Rule > >;
         static_assert( !( has_apply && is_nothing ), "unexpected apply() defined" );
         static_assert( !( has_apply0 && is_nothing ), "unexpected apply0() defined" );

         if constexpr( !has_apply && std::is_base_of_v< require_apply, Action< Rule > > ) {
            internal::missing_apply< Control< Rule >, Action >( in, st... );
         }

         if constexpr( !has_apply0 && std::is_base_of_v< require_apply0, Action< Rule > > ) {
            internal::missing_apply0< Control< Rule >, Action >( in, st... );
         }

         constexpr bool validate_nothing = std::is_base_of_v< maybe_nothing, Action< void > >;
         constexpr bool is_maybe_nothing = std::is_base_of_v< maybe_nothing, Action< Rule > >;
         static_assert( !enable_action || !validate_nothing || is_nothing || is_maybe_nothing || has_apply || has_apply0, "either apply() or apply0() must be defined" );

         constexpr bool use_guard = has_apply || has_apply0_bool;

         auto m = in.template auto_rewind< ( use_guard ? rewind_mode::required : rewind_mode::dontcare ) >();
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         auto result = internal::match_control_unwind< Rule, A, ( use_guard ? rewind_mode::active : M ), Action, Control >( in, st... );
         if( result ) {
            if constexpr( has_apply_void ) {
               Control< Rule >::template apply< Action >( m.frobnicator(), static_cast< const ParseInput& >( in ), st... );
            }
            else if constexpr( has_apply_bool ) {
               result = Control< Rule >::template apply< Action >( m.frobnicator(), static_cast< const ParseInput& >( in ), st... );
            }
            else if constexpr( has_apply0_void ) {
               Control< Rule >::template apply0< Action >( static_cast< const ParseInput& >( in ), st... );
            }
            else if constexpr( has_apply0_bool ) {
               result = Control< Rule >::template apply0< Action >( static_cast< const ParseInput& >( in ), st... );
            }
         }
         if( result ) {
            Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
         }
         else {
            Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         }
         (void)m( result );
         return result;
      }
   }

}  // namespace tao::pegtl

#if defined( _MSC_VER )
#pragma warning( pop )
#endif

#endif
