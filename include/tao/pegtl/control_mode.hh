// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_CONTROL_MODE_HH
#define TAOCPP_PEGTL_INCLUDE_CONTROL_MODE_HH

#include "config.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   enum class control_mode : bool
   {
      CONTROL = true,
      NOTHING = false
   };

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
