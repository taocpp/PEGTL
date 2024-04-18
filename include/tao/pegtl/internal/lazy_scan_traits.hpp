// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "scan_input.hpp"
#include "test_after_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Peek >
   struct lazy_scan_traits
   {
      template< typename Position, typename Data >
      static void scan( Position& pos, scan_input< Data >& in )
      {
         pos.count += in.size();

         while( !in.empty() ) {
            if( normal< Eol >::template match< apply_mode::nothing, rewind_mode::required, nothing, normal >( in ) ) {
               pos.line++;
               pos.column = 1;
               continue;
            }
            if( const auto pair = Peek::peek( in ) ) {
               pos.column += pair.size();
               in.template consume< void >( pair.size() );
               continue;
            }
            break;
         }
         test_after_scan( in );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
