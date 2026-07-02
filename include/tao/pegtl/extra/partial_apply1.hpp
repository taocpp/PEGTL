// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_PARTIAL_APPLY1_HPP
#define TAO_PEGTL_EXTRA_PARTIAL_APPLY1_HPP

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"

#include "internal/partial_apply1.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, typename... Rules >
   struct partial_apply1
      : internal::partial_apply1< Rule, Rules... >
   {};

   template< typename Name, typename Rule, typename... Rules >
   struct analyze_traits< Name, internal::partial_apply1< Rule, Rules... > >
      : analyze_traits< Name, typename internal::seq< Rule, internal::partial< Rules... > >::rule_t >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
