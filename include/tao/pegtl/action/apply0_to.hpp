// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_APPLY0_TO_HPP
#define TAO_PEGTL_ACTION_APPLY0_TO_HPP

#include <tuple>

#include "../config.hpp"

#include "require_apply0.hpp"

#include "internal/apply_to_call.hpp"
#include "internal/apply_to_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< auto S, auto... As >
   struct apply0_to
      : require_apply0
   {
      template< typename... States >
      static void apply0( States&&... st )
      {
         using call = internal::apply_to_call< S, As... >;
         using target = typename internal::apply_to_target_t< S >;
         target& out = std::get< target& >( std::tie( st... ) );
         call::call( out );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
