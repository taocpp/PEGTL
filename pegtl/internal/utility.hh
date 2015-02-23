// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_UTILITY_HH
#define PEGTL_INTERNAL_UTILITY_HH

#include <cstring>
#include <utility>
#include <algorithm>

#include "../apply_mode.hh"

#include "apply_here.hh"

namespace pegtl
{
   namespace internal
   {
      constexpr bool is_alpha( const int c )
      {
         return ( ( 'a' <= c ) && ( c <= 'z' ) ) || ( ( 'A' <= c ) && ( c <= 'Z' ) );
      }

      constexpr apply_here merge( const apply_mode a, const apply_here b )
      {
         static_assert( bool( apply_mode::ACTION ) == true, "internal consistency" );
         static_assert( bool( apply_mode::NOTHING ) == false, "internal consistency" );
         static_assert( bool( apply_here::ACTION ) == true, "internal consistency" );
         static_assert( bool( apply_here::NOTHING ) == false, "internal consistency" );
         return apply_here( bool( a ) && bool( b ) );
      }

      template< typename Char >
      bool contains( const Char c, const std::initializer_list< Char > & l )
      {
         return std::find( l.begin(), l.end(), c ) != l.end();
      }

      inline bool unsafe_equals( const char * s, const std::initializer_list< char > & l )
      {
         return std::memcmp( s, & * l.begin(), l.size() ) == 0;
      }

   } // internal

} // pegtl

#endif
