// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_STATE_CONTROL_HPP
#define TAO_PEGTL_CONTROL_STATE_CONTROL_HPP

#include "../config.hpp"
#include "../normal.hpp"

#include "internal/state_control.hpp"

#include "rotate_states_right.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // state_control_b is not quite straightforward

   template< template< typename... > class Control, typename Rule >
   using state_control_r = rotate_states_right_b< 1, internal::state_control< Control, Rule > >;

   template< template< typename... > class Control = normal >
   struct state_control_n
   {
      template< typename Rule >
      using type = state_control_r< Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
