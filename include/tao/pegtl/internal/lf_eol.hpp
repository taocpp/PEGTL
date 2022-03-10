// Copyright (c) 2016-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LF_EOL_HPP
#define TAO_PEGTL_INTERNAL_LF_EOL_HPP

#include "data_and_size.hpp"

namespace tao::pegtl::internal
{
   struct lf_eol
   {
      static constexpr int ch = '\n';

      template< typename ParseInput >
      [[nodiscard]] static bool_and_size eol_match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         bool_and_size p = { false, in.size( 1 ) };
         if( p.size > 0 ) {
            if( in.peek_char() == '\n' ) {
               in.bump_to_next_line();
               p.data = true;
            }
         }
         return p;
      }
   };

}  // namespace tao::pegtl::internal

#endif
