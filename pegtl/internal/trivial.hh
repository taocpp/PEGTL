// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_TRIVIAL_HH
#define PEGTL_INTERNAL_TRIVIAL_HH

namespace pegtl
{
   namespace internal
   {
      template< bool What >
      struct trivial
      {
         using internal_t = trivial;

         template< typename Input >
         static bool match( Input & )
         {
            return What;
         }
      };

   } // internal

} // pegtl

#endif
