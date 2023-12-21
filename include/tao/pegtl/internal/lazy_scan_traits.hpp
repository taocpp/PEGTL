// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename = void >
   struct lazy_scan_traits
   {
      template< typename Input, typename Position >
      static void scan( Input& in, Position& pos )
      {
         while( !in.empty() ) {
            if( normal< Eol >::template match< apply_mode::nothing, rewind_mode::required, nothing, normal >( in ) ) {
               pos.line++;
               pos.column = 1;
            }
            else {
               pos.column++;
               in.template consume< void >( 1 );
            }
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
