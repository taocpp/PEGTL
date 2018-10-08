// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_APPLY_SINGLE_HPP
#define TAO_PEGTL_INTERNAL_APPLY_SINGLE_HPP

#include "../config.hpp"

#include <type_traits>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Action >
         struct apply_single
         {
            template< typename Input, typename... States >
            static auto match( const Input& in, States&&... st ) noexcept( noexcept( Action::apply( in, st... ) ) )
               -> std::enable_if_t< std::is_same_v< decltype( Action::apply( in, st... ) ), void >, bool >
            {
               Action::apply( in, st... );
               return true;
            }

            template< typename Input, typename... States >
            static auto match( const Input& in, States&&... st ) noexcept( noexcept( Action::apply( in, st... ) ) )
               -> std::enable_if_t< std::is_same_v< decltype( Action::apply( in, st... ) ), bool >, bool >
            {
               return Action::apply( in, st... );
            }
         };

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
