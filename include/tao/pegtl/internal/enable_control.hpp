// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ENABLE_CONTROL_HPP
#define TAO_PEGTL_INTERNAL_ENABLE_CONTROL_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule >
   inline constexpr bool enable_control = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
