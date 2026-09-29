// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_UNHEX_UTILITY_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_UNHEX_UTILITY_HPP

#include <cstddef>

#include "../../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename C >
   [[nodiscard]] constexpr C unhex_char( const char c ) noexcept
   {
      switch( c ) {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            return C( c - '0' );
         case 'a':
         case 'b':
         case 'c':
         case 'd':
         case 'e':
         case 'f':
            return C( c - 'a' + 10 );
         case 'A':
         case 'B':
         case 'C':
         case 'D':
         case 'E':
         case 'F':
            return C( c - 'A' + 10 );
         default:             // LCOV_EXCL_LINE
            return C( 127 );  // LCOV_EXCL_LINE
      }
   }

   template< std::size_t N, typename C >
   [[nodiscard]] constexpr C unhex_string( const char* begin ) noexcept
   {
      static_assert( N >= 1 );
      static_assert( N <= sizeof( C ) * 2 );

      C result = unhex_char< C >( begin[ 0 ] );
      for( std::size_t i = 1; i < N; ++i ) {
         result <<= 4;  // NOLINT
         result += unhex_char< C >( begin[ i ] );
      }
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
