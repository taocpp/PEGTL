// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INVERT_MODE_HPP
#define TAO_PEGTL_INTERNAL_INVERT_MODE_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   enum class invert_mode : bool
   {
      enabled = true,
      disabled = false
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
