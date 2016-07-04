// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_BUMP_UTIL_HH
#define PEGTL_INTERNAL_BUMP_UTIL_HH

#include <type_traits>

#include "result_on_found.hh"

namespace pegtl
{
   namespace internal
   {
      template< bool > struct bump_impl;

      template<> struct bump_impl< true >
      {
         template< typename Input >
         static void bump( Input & in, const std::size_t count )
         {
            in.bump( count );
         }
      };

      template<> struct bump_impl< false >
      {
         template< typename Input >
         static void bump( Input & in, const std::size_t count )
         {
            in.bump_in_this_line( count );
         }
      };

      template< bool ... > struct bool_list {};
      template< bool ... Bs > using bool_and = std::is_same< bool_list< Bs..., true >, bool_list< true, Bs... > >;

      template< result_on_found R, typename Input, typename Char, Char ... Cs >
      void bump( Input & in, const std::size_t count )
      {
         bump_impl< bool_and< ( Cs != '\n' ) ... >::value != bool( R ) >::bump( in, count );
      }

   } // internal

} // pegtl

#endif
