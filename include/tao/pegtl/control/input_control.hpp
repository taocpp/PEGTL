// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_INPUT_CONTROL_HPP
#define TAO_PEGTL_CONTROL_INPUT_CONTROL_HPP

#include "../config.hpp"
#include "../normal.hpp"

#include "internal/input_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // input_control_b is not quite straightforward

   template< template< typename... > class Control, typename Rule >
   using input_control_r = internal::input_control< Control, Rule >;

   template< template< typename... > class Control = normal >
   struct input_control_n
   {
      template< typename Rule >
      using type = internal::input_control< Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
