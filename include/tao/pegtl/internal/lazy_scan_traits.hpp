// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_LAZY_SCAN_TRAITS_HPP

#include <cstddef>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "lazy_scan_input.hpp"
#include "one.hpp"
#include "scan_utility.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol >
   struct lazy_scan_traits
   {
      template< typename Position, typename Data >
      static void scan( Position& pos, const Data* data, const std::size_t count )
      {
         lazy_scan_input< Data > in( data, count );

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
         pos.count += count;
      }
   };

   template< char Eol, typename Peek >
   struct lazy_scan_traits< single< one< Peek, Eol > > >
      : scan_char_impl< Eol >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
