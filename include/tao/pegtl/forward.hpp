// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_FORWARD_HPP
#define TAO_PEGTL_FORWARD_HPP

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name, typename Rule, typename = void >
   struct analyze_traits;

   template< typename Left, typename Right, typename = void >
   struct combine_traits;

   template< typename Rule, typename = void >
   struct invert_traits;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
