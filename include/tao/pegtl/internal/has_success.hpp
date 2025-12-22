// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_HAS_SUCCESS_HPP
#define TAO_PEGTL_INTERNAL_HAS_SUCCESS_HPP

#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... >
   inline constexpr bool has_success = false;

   template< typename State, typename... Ts >
   inline constexpr bool has_success< State, decltype( std::declval< State >().success( std::declval< Ts >()... ) ), Ts... > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
