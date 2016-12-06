// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_LF_EOL_HH
#define PEGTL_LF_EOL_HH

namespace pegtl
{
   struct lf_eol
   {
      static constexpr int ch = '\n';

      template< typename Input >
      static eol_pair match( Input & in )
      {
         eol_pair p = { false, in.size( 1 ) };
         if ( p.second ) {
            if ( in.peek_char() == '\n' ) {
               in.bump_to_next_line();
               p.first = true;
            }
         }
         return p;
      }
   };

} // namespace pegtl

#endif
