// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_ANY_HH
#define PEGTL_INTERNAL_ANY_HH

#include "peek_char.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Peek >
      struct any
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( in.size() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  in.bump( t.size );
                  return true;
               }
            }
            return false;
         }
      };

      template<> struct any< peek_char >
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            return in.bump_if();
         }
      };

   } // internal

} // pegtl

#endif
