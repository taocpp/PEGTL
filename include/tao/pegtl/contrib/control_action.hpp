// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_CONTROL_ACTION_HPP
#define TAO_PEGTL_CONTRIB_CONTROL_ACTION_HPP

#include <utility>

#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename, typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_start = false;

      template< typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_start< decltype( (void)Action< Rule >::start( std::declval< const ParseInput& >(), std::declval< States&& >()... ) ), Rule, Action, ParseInput, States... > = true;

      template< typename, typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_success = false;

      template< typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_success< decltype( (void)Action< Rule >::success( std::declval< const ParseInput& >(), std::declval< States&& >()... ) ), Rule, Action, ParseInput, States... > = true;

      template< typename, typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_failure = false;

      template< typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_failure< decltype( (void)Action< Rule >::failure( std::declval< const ParseInput& >(), std::declval< States&& >()... ) ), Rule, Action, ParseInput, States... > = true;

      template< typename, typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_unwind = false;

      template< typename Rule, template< typename... > class Action, typename ParseInput, typename... States >
      inline constexpr bool action_has_unwind< decltype( (void)Action< Rule >::unwind( std::declval< const ParseInput& >(), std::declval< States&& >()... ) ), Rule, Action, ParseInput, States... > = true;

   }  // namespace internal

   struct control_action
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
         static_assert( internal::action_has_start< void, Rule, Action, ParseInput, States... > || internal::action_has_success< void, Rule, Action, ParseInput, States... > || internal::action_has_failure< void, Rule, Action, ParseInput, States... > || internal::action_has_unwind< void, Rule, Action, ParseInput, States... >, "at least one function should be found for this to make sense" );

         if constexpr( internal::action_has_unwind< void, Rule, Action, ParseInput, States... > ) {
            try {
               return control_action::match_impl< Rule, A, M, Action, Control >( in, st... );
            }
            catch( ... ) {
               Action< Rule >::unwind( const_cast< const ParseInput& >( in ), st... );
               throw;
            }
         }
         else {
            return control_action::match_impl< Rule, A, M, Action, Control >( in, st... );
         }
      }

   private:
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match_impl( ParseInput& in, States&&... st )
      {
         if constexpr( internal::action_has_start< void, Rule, Action, ParseInput, States... > ) {
            Action< Rule >::start( const_cast< const ParseInput& >( in ), st... );
         }
         if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... ) ) {
            if constexpr( internal::action_has_success< void, Rule, Action, ParseInput, States... > ) {
               Action< Rule >::success( const_cast< const ParseInput& >( in ), st... );
            }
            return true;
         }
         if constexpr( internal::action_has_failure< void, Rule, Action, ParseInput, States... > ) {
            Action< Rule >::failure( const_cast< const ParseInput& >( in ), st... );
         }
         return false;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
