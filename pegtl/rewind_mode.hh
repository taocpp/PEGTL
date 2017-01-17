// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_REWIND_MODE_HH
#define PEGTL_REWIND_MODE_HH

#include "config.hh"

namespace PEGTL_NAMESPACE
{
   enum class rewind_mode : char
   {
      ACTIVE,
      REQUIRED,
      DONTCARE
   };

} // namespace PEGTL_NAMESPACE

#endif
