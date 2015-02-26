// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_STRING_HH
#define PEGTL_INTERNAL_STRING_HH

#include <utility>
#include <cstring>

#include "trivial.hh"

#include "../analysis/counted.hh"

namespace pegtl
{
   namespace internal
   {
      inline bool unsafe_equals( const char * s, const std::initializer_list< char > & l )
      {
         return std::memcmp( s, & * l.begin(), l.size() ) == 0;
      }

      template< char ... Cs > struct string;

      template<> struct string<>
            : trivial< true > {};

      template< char C, char ... Cs >
      struct string< C, Cs ... >
      {
         using analyze_t = analysis::counted< analysis::rule_type::ANY, 1 + sizeof ... ( Cs ) >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( in.size() >= 1 + sizeof ... ( Cs ) ) {
               if ( unsafe_equals( in.begin(), { C, Cs ... } ) ) {
                  in.bump( 1 + sizeof ... ( Cs ) );
                  return true;
               }
            }
            return false;
         }
      };

   } // internal

} // pegtl

#endif
