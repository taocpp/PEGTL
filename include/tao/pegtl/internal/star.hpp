// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_STAR_HPP
#define TAO_PEGTL_INTERNAL_STAR_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "seq.hpp"
#include "star_partial.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename... Rules >
   struct star
      : star< seq< Rule, Rules... > >
   {};

   template< typename Rule >
   struct star< Rule >
      : star_partial< Rule >
   {
      using rule_t = star;
   };

   template< typename Rule, typename... Rules >
   inline constexpr bool enable_control< star< Rule, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif  // FCOV_EXCL_FILE
