// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_ADD_STATE_HPP
#define TAO_PEGTL_ACTION_ADD_STATE_HPP

#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "../internal/dependent_false.hpp"
#include "../internal/type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename AddState >
   struct add_state
      : maybe_nothing
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
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( std::is_default_constructible_v< AddState > ) {
            AddState s;
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, s, st... ) ) {
               if constexpr( A == apply_mode::action ) {
                  Action< Rule >::success( static_cast< const ParseInput& >( in ), s, st... );
               }
               return true;
            }
            return false;
         }
         else if constexpr( std::is_constructible_v< AddState, const ParseInput&, States... > ) {
            AddState s( static_cast< const ParseInput& >( in ), st... );
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, s, st... ) ) {
               if constexpr( A == apply_mode::action ) {
                  Action< Rule >::success( static_cast< const ParseInput& >( in ), s, st... );
               }
               return true;
            }
            return false;
         }
         else {
            static_assert( internal::dependent_false< AddState >, "Unable to instantiate new state!" );
         }
      }

      template< typename ParseInput,
                typename... States >
      static void success( const ParseInput& in, AddState& s, States&&... st )
      {
         if constexpr( internal::has_success< AddState, void, const ParseInput&, States... > ) {
            s.success( in, st... );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
