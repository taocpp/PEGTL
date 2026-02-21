// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_APPLY_TYPED_STATE_HPP
#define TAO_PEGTL_CONTROL_APPLY_TYPED_STATE_HPP

#include "../config.hpp"
#include "../normal.hpp"

#include "internal/apply_typed_state.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename State, typename Base >
   using apply_typed_state_b = internal::apply_typed_state< State, Base >;

   template< typename State, template< typename... > class Control, typename Rule >
   using apply_typed_state_r = apply_typed_state_b< State, Control< Rule > >;

   template< typename State, template< typename... > class Control = normal >
   struct apply_typed_state_n
   {
      template< typename Rule >
      using type = apply_typed_state_r< State, Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
