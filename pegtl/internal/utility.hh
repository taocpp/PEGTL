// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_UTILITY_HH
#define PEGTL_INTERNAL_UTILITY_HH

#include <cstring>
#include <utility>
#include <algorithm>

namespace pegtl
{
   namespace internal
   {
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
