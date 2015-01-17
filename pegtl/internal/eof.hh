// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_EOF_HH
#define PEGTL_INTERNAL_EOF_HH

namespace pegtl
{
   namespace internal
   {
      struct eof
      {
         using internal_t = eof;

         template< typename Input >
         static bool match( Input & in )
         {
            return in.empty();
         }
      };

   } // internal

} // pegtl

#endif
