// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
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
            if( ( unsigned( *in.current() ) % M ) == R ) {
               in.template consume< my_rule >( 1 );
               return true;
            }
         }
         return false;
      }
   };

   struct grammar
      : pegtl::seq< my_rule< 3 >, pegtl::eof >
   {};

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      const std::string arg( argv[ i ] );
      std::cout << "   '" << arg;
      if( arg.size() != 1 ) {
         std::cout << "' does not have length 1" << std::endl;
         continue;
      }
      pegtl::argv_input in( argv, i );
      if( pegtl::parse< example::grammar >( in ) ) {
         std::cout << "' is a match" << std::endl;
      }
      else {
         std::cout << "' is NOT a match" << std::endl;
      }
   }
   return 0;
}
