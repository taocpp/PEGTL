// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_HIERARCHICAL_HPP
#define TAO_PEGTL_CONTRIB_HIERARCHICAL_HPP

#include "../config.hpp"
#include "../normal.hpp"

#include <type_traits>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      // Base class to use as tag
      struct hierarchical_action_tag
      {};

      namespace internal
      {
         // Detect if the class derives from hierarchical action and therefore is a hierarchical action
         template< typename T >
         constexpr bool is_hierarchical_action = std::is_base_of_v< hierarchical_action_tag, T >;
      }  // namespace internal

      // Control class that dispatches on hierarchical action
      template< class Rule >
      struct hierarchical_control : normal< Rule >
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            using action_t = Action< Rule >;
            if constexpr( internal::is_hierarchical_action< action_t > ) {
               return action_t::template match< A, M, Action, Control >( in, st... );
            }
            else {
               return normal< Rule >::template match< A, M, Action, Control >( in, st... );
            }
         }
      };

      // Change action when starting to match rule
      template< typename Rule, template< typename... > class NewAction >
      struct hierarchical_change_action : hierarchical_action_tag
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            return Control< Rule >::template match< A, M, NewAction, Control >( in, st... );
         }
      };

      // Change control and use new control to match rule
      template< typename Rule, template< typename... > class NewControl >
      struct hierarchical_change_control : hierarchical_action_tag
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            return Control< Rule >::template match< A, M, Action, NewControl >( in, st... );
         }
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
