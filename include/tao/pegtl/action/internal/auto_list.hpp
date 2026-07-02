// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_AUTO_LIST_HPP
#define TAO_PEGTL_ACTION_INTERNAL_AUTO_LIST_HPP

#include "../../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< auto... As >
   struct auto_list
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
