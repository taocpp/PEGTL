// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REP_MIN_HPP
#define TAO_PEGTL_INTERNAL_REP_MIN_HPP

#include "rep.hpp"
#include "seq.hpp"
#include "star.hpp"

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Min, typename Rule, typename... Rules >
   using rep_min = seq< rep< Min, Rule, Rules... >, star< Rule, Rules... > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
