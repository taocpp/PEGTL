// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_CHANGES_HPP
#define TAO_PEGTL_CONTRIB_CHANGES_HPP

#include <type_traits>

#include "../config.hpp"
#include "../normal.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule, typename State, template< typename... > class Base = normal >
      struct change_state
         : public Base< Rule >
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
            State s( static_cast< const Input& >( in ), st... );
            if( Base< Rule >::template match< A, M, Action, Control >( in, s ) ) {
               if constexpr( A == apply_mode::action ) {
                  s.success( static_cast< const Input& >( in ), st... );
               }
               return true;
            }
            return false;
         }
      };

      template< typename Rule, template< typename... > class Action, template< typename... > class Base = normal >
      struct change_action
         : public Base< Rule >
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            return Base< Rule >::template match< A, M, Action, Control >( in, st... );
         }
      };

      template< template< typename... > class Action, template< typename... > class Base >
      struct change_both_helper
      {
         template< typename T >
         using change_action = change_action< T, Action, Base >;
      };

      template< typename Rule, typename State, template< typename... > class Action, template< typename... > class Base = normal >
      struct change_state_and_action
         : change_state< Rule, State, change_both_helper< Action, Base >::template change_action >
      {
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
