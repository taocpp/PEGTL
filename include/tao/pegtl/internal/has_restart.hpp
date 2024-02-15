// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_HAS_RESTART_HPP
#define TAO_PEGTL_INTERNAL_HAS_RESTART_HPP

#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input, typename = void >
   inline constexpr bool has_restart = false;

   template< typename Input >
   inline constexpr bool has_restart< Input, decltype( (void)std::declval< Input >().restart() ) > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
