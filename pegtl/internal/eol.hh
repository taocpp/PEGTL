// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_EOL_HH
#define PEGTL_INTERNAL_EOL_HH

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      struct eol
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( const auto s = in.size() ) {
               return match_impl( in, s );
            }
            return false;
         }

         template< typename Input >
         static bool match_impl( Input & in, const std::size_t s )
         {
            const auto a = in.peek_char();
            if ( a == '\n' ) {
               in.bump_next_line();
               return true;
            }
            if ( ( a == '\r' ) && ( s > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) {
               in.bump_next_line( 2 );
               return true;
            }
            return false;
         }
      };

      template<>
      struct skip_control< eol > : std::true_type {};

   } // internal

} // pegtl

#endif
