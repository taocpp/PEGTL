// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEPENDENT_FALSE_HPP
#define TAO_PEGTL_INTERNAL_DEPENDENT_FALSE_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... >
   inline constexpr bool dependent_false = false;

   template< typename... Ts >
   struct static_assert_dependent_false
   {
      static constexpr bool value = dependent_false< Ts... >;
      static_assert( value );
   };

   template< typename... Ts >
   inline constexpr bool static_assert_dependent_false_v = static_assert_dependent_false< Ts... >::value;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
