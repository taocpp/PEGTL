// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SKIP_CONTROL_HH
#define PEGTL_INTERNAL_SKIP_CONTROL_HH

#include <type_traits>

namespace pegtl
{
   namespace internal
   {
      template< typename Rule >
      struct skip_control : std::false_type {};

   } // internal

} // pegtl

#endif
