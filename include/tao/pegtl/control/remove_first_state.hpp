// Copyright (c) 2019-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REMOVE_FIRST_STATE_HPP
#define TAO_PEGTL_CONTROL_REMOVE_FIRST_STATE_HPP

#include "../config.hpp"

#include "internal/remove_first_state.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Base >
   using remove_first_state_b = internal::remove_first_state< Base >;

   template< template< typename... > class Control, typename Rule >
   using remove_first_state_r = remove_first_state_b< Control< Rule > >;

   template< template< typename... > class Control >
   struct remove_first_state_n
   {
      template< typename Rule >
      using type = remove_first_state_r< Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
