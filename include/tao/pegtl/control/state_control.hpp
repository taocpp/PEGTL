// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_STATE_CONTROL_HPP
#define TAO_PEGTL_CONTROL_STATE_CONTROL_HPP

#include <type_traits>

#include "../config.hpp"

#include "../internal/type_traits.hpp"

#include "shuffle_states.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< template< typename... > class Control >
   struct state_control
   {
      template< typename Rule >
      struct impl
         : Control< Rule >
      {
         static constexpr bool enable = true;

         template< typename ParseInput, typename State, typename... States >
         static void start( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] State& state, [[maybe_unused]] States&&... st )
         {
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::start( in, st... );
            }
            if constexpr( State::template enable< Rule > ) {
               state.template start< Rule >( in, st... );
            }
#if defined( _MSC_VER )
            ( (void)st,
              ... );
#endif
         }

         template< typename ParseInput, typename State, typename... States >
         static void success( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] State& state, [[maybe_unused]] States&&... st )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template success< Rule >( in, st... );
            }
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::success( in, st... );
            }
#if defined( _MSC_VER )
            ( (void)st,
              ... );
#endif
         }

         template< typename ParseInput, typename State, typename... States >
         static void failure( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] State& state, [[maybe_unused]] States&&... st )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template failure< Rule >( in, st... );
            }
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::failure( in, st... );
            }
#if defined( _MSC_VER )
            ( (void)st,
              ... );
#endif
         }

         // TODO: static auto guard() ... ?

         template< typename ParseInput, typename State, typename... States >
         [[noreturn]] static void raise( const ParseInput& in, [[maybe_unused]] State& state, States&&... st )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template raise< Rule >( in, st... );
            }
            Control< Rule >::raise( in, st... );
         }

         template< typename Ambient, typename ParseInput, typename State, typename... States >
         [[noreturn]] static void raise_nested( const Ambient& am, const ParseInput& in, [[maybe_unused]] State& state, States&&... st )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template raise_nested< Rule >( am, in, st... );
            }
            Control< Rule >::raise_nested( am, in, st... );
         }

         template< typename ParseInput, typename State, typename... States >
         static auto unwind( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] State& state, [[maybe_unused]] States&&... st )
            -> std::enable_if_t< State::template enable< Rule > || ( Control< Rule >::enable && internal::has_unwind< Control< Rule >, void, const ParseInput&, States... > ) >
         {
            if constexpr( State::template enable< Rule > ) {
               state.template unwind< Rule >( in, st... );
            }
            if constexpr( Control< Rule >::enable && internal::has_unwind< Control< Rule >, void, const ParseInput&, States... > ) {
               Control< Rule >::unwind( in, st... );
            }
#if defined( _MSC_VER )
            ( (void)st,
              ... );
#endif
         }

         template< template< typename... > class Action, typename RewindPosition, typename ParseInput, typename State, typename... States >
         static auto apply( const RewindPosition& begin, const ParseInput& in, [[maybe_unused]] State& state, States&&... st )
            -> decltype( Control< Rule >::template apply< Action >( begin, in, st... ) )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template apply< Rule >( in, st... );
            }
            return Control< Rule >::template apply< Action >( begin, in, st... );
         }

         template< template< typename... > class Action, typename ParseInput, typename State, typename... States >
         static auto apply0( const ParseInput& in, [[maybe_unused]] State& state, States&&... st )
            -> decltype( Control< Rule >::template apply0< Action >( in, st... ) )
         {
            if constexpr( State::template enable< Rule > ) {
               state.template apply0< Rule >( in, st... );
            }
            return Control< Rule >::template apply0< Action >( in, st... );
         }
      };

      template< typename Rule >
      using type = rotate_states_right< impl< Rule > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
