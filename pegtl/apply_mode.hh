// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_APPLY_MODE_HH
#define PEGTL_APPLY_MODE_HH

#include "config.hh"

namespace PEGTL_NAMESPACE
{
   enum class apply_mode : bool
   {
      ACTION = true,
      NOTHING = false
   };

} // namespace PEGTL_NAMESPACE

#endif
