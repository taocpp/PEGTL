// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IS_NOTHING_HH
#define PEGTL_INTERNAL_IS_NOTHING_HH

#include <type_traits>

#include "../nothing.hh"

namespace pegtl
{
   namespace internal
   {
      // Helper to detect whether an action class template is actually
      // used for a specific rule or not.

      template< template< typename ... > class Action, typename Rule >
      using is_nothing = std::is_base_of< nothing< Rule >, Action< Rule > >;

   } // internal

} // pegtl

#endif
