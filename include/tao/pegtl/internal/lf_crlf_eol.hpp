// Copyright (c) 2016-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LF_CRLF_EOL_HPP
#define TAO_PEGTL_INTERNAL_LF_CRLF_EOL_HPP

#include "data_and_size.hpp"

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct lf_crlf_eol
   {
      static constexpr int ch = '\n';

      template< typename ParseInput >
      [[nodiscard]] static bool_and_size eol_match( ParseInput& in ) noexcept( noexcept( in.size( 2 ) ) )
      {
         bool_and_size p = { false, in.size( 2 ) };
         if( p.size > 0 ) {
            const auto a = in.peek_char();
            if( a == '\n' ) {
               in.bump_to_next_line();
               p.size = 1;
               p.data = true;
            }
            else if( ( a == '\r' ) && ( p.size > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) {
               in.bump_to_next_line( 2 );
               p.size = 2;
               p.data = true;
            }
         }
         return p;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
