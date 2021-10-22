// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl.hpp>

using namespace TAO_PEGTL_NAMESPACE;

namespace modulus
{
   template< unsigned M, unsigned R = 0 >
   struct my_rule
   {
      static_assert( M > 1, "Modulus must be greater than 1" );
      static_assert( R < M, "Remainder must be less than modulus" );

      template< typename ParseInput >
      static bool match( ParseInput& in )
      {
         if( !in.empty() ) {
            if( ( ( *in.current() ) % M ) == R ) {
               in.bump( 1 );
               return true;
            }
         }
         return false;
      }
   };

   struct grammar
      : until< eolf, my_rule< 3 > >
   {};

}  // namespace modulus

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc > 1 ) {
      argv_input in( argv, 1 );
      if( !parse< modulus::grammar >( in ) ) {
         return 1;
      }
   }
   return 0;
}
