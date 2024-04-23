// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNHEX_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_UNHEX_UTILITY_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename I >
   [[nodiscard]] constexpr I unhex_char_impl( const char c ) noexcept
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
            return I( c - '0' );
         case 'a':
         case 'b':
         case 'c':
         case 'd':
         case 'e':
         case 'f':
            return I( c - 'a' + 10 );
         case 'A':
         case 'B':
         case 'C':
         case 'D':
         case 'E':
         case 'F':
            return I( c - 'A' + 10 );
         default:            // LCOV_EXCL_LINE
            return I( -1 );  // LCOV_EXCL_LINE
      }
   }

   template< typename I >
   [[nodiscard]] constexpr I unhex_string_impl( const char* begin, const char* end )
   {
      I result = 0;
      while( begin != end ) {
         result <<= 4;
         result += unhex_char_impl< I >( *begin++ );
      }
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
