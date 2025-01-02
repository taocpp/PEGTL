// Copyright (c) 2019-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_CHANGE_ACTION_AND_STATES_HPP
#define TAO_PEGTL_ACTION_CHANGE_ACTION_AND_STATES_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< template< typename... > class NewAction, typename... NewStates >
   struct change_action_and_states
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                std::size_t... Ns,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( std::index_sequence< Ns... > /*unused*/, ParseInput& in, States&&... st )
      {
         auto t = std::tie( st... );
         if( Control< Rule >::template match< A, M, NewAction, Control >( in, std::get< Ns >( t )... ) ) {
            if constexpr( A == apply_mode::action ) {
               Action< Rule >::success( static_cast< const ParseInput& >( in ), st... );
            }
            return true;
         }
         return false;
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
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         static_assert( !std::is_same_v< Action< void >, NewAction< void > >, "Old and new action class templates coincide!" );
         return match< Rule, A, M, Action, Control >( std::index_sequence_for< NewStates... >(), in, NewStates()..., st... );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
