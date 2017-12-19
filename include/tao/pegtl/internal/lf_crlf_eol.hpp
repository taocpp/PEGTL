// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_LF_CRLF_EOL_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_LF_CRLF_EOL_HPP

#include "../config.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         struct lf_crlf_eol
         {
            static constexpr int ch = '\n';

            template< typename Input >
            static eol_pair match( Input& in )
            {
               eol_pair p = { false, in.size( 2 ) };
               if( p.second ) {
                  const auto a = in.peek_char();
                  if( a == '\n' ) {
                     in.bump_to_next_line();
                     p.first = true;
                  }
                  else if( ( a == '\r' ) && ( p.second > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) {
                     in.bump_to_next_line( 2 );
                     p.first = true;
                  }
               }
               return p;
            }
         };

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
