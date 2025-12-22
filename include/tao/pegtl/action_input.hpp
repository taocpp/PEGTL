// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INPUT_HPP
#define TAO_PEGTL_ACTION_INPUT_HPP

#include "config.hpp"

#include "internal/action_input.hpp"
#include "internal/input_with_funcs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename ParseInput >
   struct action_input
      : internal::input_with_funcs< internal::action_input< ParseInput > >
   {
      using internal::input_with_funcs< internal::action_input< ParseInput > >::input_with_funcs;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
