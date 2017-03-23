// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_APPLY_HERE_HH
#define TAOCPP_PEGTL_INCLUDE_APPLY_HERE_HH

#include "config.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   enum class apply_here : char
   {
      NOTHING,
      ACTION_NO_DATA,
      ACTION_WITH_DATA
   };

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
