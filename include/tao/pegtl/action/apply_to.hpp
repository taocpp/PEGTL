// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_APPLY_TO_HPP
#define TAO_PEGTL_ACTION_APPLY_TO_HPP

#include "../config.hpp"

#include "internal/apply_to_impl.hpp"
#include "internal/apply_to_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< auto S >
   struct apply_to
      : internal::apply_to_impl< S, internal::apply_to_target_t< S >, internal::apply_to_object_t< S > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
