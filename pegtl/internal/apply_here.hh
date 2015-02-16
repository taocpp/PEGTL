// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_APPLY_HERE_HH
#define PEGTL_INTERNAL_APPLY_HERE_HH

namespace pegtl
{
   namespace internal
   {
      enum class apply_here : bool
      {
         ACTION = true,
         NOTHING = false
      };

   } // internal

} // pegtl

#endif
