// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EOL_EXCLUDE_TAG_HPP
#define TAO_PEGTL_EOL_EXCLUDE_TAG_HPP

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct eol_exclude_tag
   {
      using rule_t = eol_exclude_tag;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
