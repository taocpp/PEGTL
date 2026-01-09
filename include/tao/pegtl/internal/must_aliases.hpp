// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MUST_ALIASES_HPP
#define TAO_PEGTL_INTERNAL_MUST_ALIASES_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "star.hpp"

#include "if_must.hpp"
#include "if_then_else.hpp"
#include "must.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Cond, typename Then, typename Else >
   using if_must_else = if_then_else< Cond, must< Then >, must< Else > >;

   template< typename Rule, typename Sep >
   using list_must = seq< Rule, star< Sep, must< Rule > > >;

   template< typename Cond, typename... Rules >
   using star_must = star< if_must< false, Cond, Rules... > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
