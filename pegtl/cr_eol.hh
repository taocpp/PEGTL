// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CR_EOL_HH
#define PEGTL_CR_EOL_HH

namespace pegtl
{
   struct cr_eol
   {
      static constexpr int ch = '\r';

      template< typename Input >
      static eol_pair match( Input & in )
      {
         eol_pair p = { false, in.size( 1 ) };
         if ( p.second ) {
            if ( in.peek_char() == '\r' ) {
               in.bump_to_next_line();
               p.first = true;
            }
         }
         return p;
      }
   };

} // namespace pegtl

#endif
