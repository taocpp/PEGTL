// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PAD_HPP
#define TAO_PEGTL_INTERNAL_PAD_HPP

#include "seq.hpp"
#include "star.hpp"

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Pad1 >
   using lpad = seq< star< Pad1 >, Rule >;

   template< typename Rule, typename Pad1, typename Pad2 = Pad1 >
   using pad = seq< star< Pad1 >, Rule, star< Pad2 > >;

   template< typename Rule, typename Pad2 >
   using rpad = seq< Rule, star< Pad2 > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
