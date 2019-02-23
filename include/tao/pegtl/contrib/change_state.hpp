// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CHANGE_STATE_HPP
#define TAO_PEGTL_CHANGE_STATE_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename State >
      struct change_state
      {
         template< typename Rule,
                   apply_mode A,
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
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, s ) ) {
               if constexpr( A == apply_mode::action ) {
                  s.success( static_cast< const Input& >( in ), st... );
               }
               return true;
            }
            return false;
         }
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
