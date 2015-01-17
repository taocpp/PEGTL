// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_AT_ONE_HH
#define PEGTL_INTERNAL_AT_ONE_HH

#include "utility.hh"

namespace pegtl
{
   namespace internal
   {
      template< bool Want, typename Peek, typename Peek::data_t ... Cs >
      struct at_one
      {
         using internal_t = at_one;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( in.size() ) {
               const auto t = Peek::peek( in );
               return t && ( Want == internal::contains( t.data, { Cs ... } ) );
            }
            return false;
         }
      };

   } // internal

} // pegtl

#endif
