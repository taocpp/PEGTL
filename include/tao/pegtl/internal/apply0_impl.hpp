// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_APPLY0_IMPL_HPP
#define TAO_PEGTL_INTERNAL_APPLY0_IMPL_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Action >
   struct apply0_impl
   {
      template< typename... States >
      [[nodiscard]] static auto apply0( States&&... st ) noexcept( noexcept( Action::apply0( st... ) ) )
         -> std::enable_if_t< std::is_same_v< decltype( Action::apply0( st... ) ), void >, bool >
      {
         Action::apply0( st... );
         return true;
      }

      template< typename... States >
      [[nodiscard]] static auto apply0( States&&... st ) noexcept( noexcept( Action::apply0( st... ) ) )
         -> std::enable_if_t< std::is_same_v< decltype( Action::apply0( st... ) ), bool >, bool >
      {
         return Action::apply0( st... );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
