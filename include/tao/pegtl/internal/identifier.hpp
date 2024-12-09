// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IDENTIFIER_HPP
#define TAO_PEGTL_INTERNAL_IDENTIFIER_HPP

#include "../config.hpp"

#include "ranges.hpp"
#include "seq.hpp"
#include "star.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek >
   using identifier_first = ranges< Peek, 'a', 'z', 'A', 'Z', '_' >;
   template< typename Peek >
   using identifier_other = ranges< Peek, 'a', 'z', 'A', 'Z', '0', '9', '_' >;
   template< typename Peek >
   using identifier = seq< identifier_first< Peek >, star< identifier_other< Peek > > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
