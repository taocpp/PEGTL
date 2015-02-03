// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_EOF_HH
#define PEGTL_INTERNAL_EOF_HH

#include "../analysis/rule_class.hh"

namespace pegtl
{
   namespace internal
   {
      struct eof
      {
         using analyze_t = analysis::optional<>;

         template< typename Input >
         static bool match( Input & in )
         {
            return in.empty();
         }
      };

   } // internal

} // pegtl

#endif
