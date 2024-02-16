// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_ADD_GUARD_HPP
#define TAO_PEGTL_ACTION_ADD_GUARD_HPP

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
   template< typename AddGuard >
   struct add_guard
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
         if constexpr( std::is_default_constructible_v< AddGuard > ) {
            AddGuard g;
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... ) ) {
               if constexpr( A == apply_mode::action ) {
                  Action< Rule >::success( static_cast< const ParseInput& >( in ), g, st... );
               }
               return true;
            }
            return false;
         }
         else if constexpr( std::is_constructible_v< AddGuard, const ParseInput&, States... > ) {
            AddGuard g( static_cast< const ParseInput& >( in ), st... );
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... ) ) {
               if constexpr( A == apply_mode::action ) {
                  Action< Rule >::success( static_cast< const ParseInput& >( in ), g, st... );
               }
               return true;
            }
            return false;
         }
         else {
            static_assert( internal::dependent_false< AddGuard >, "Unable to instantiate guard!" );
         }
      }

      template< typename ParseInput,
                typename... States >
      static void success( const ParseInput& in, AddGuard& g, States&&... st )
      {
         if constexpr( internal::has_success< AddGuard, void, const ParseInput&, States... > ) {
            g.success( in, st... );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
