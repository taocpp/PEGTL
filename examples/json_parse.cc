// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <iostream>

#include "json_errors.hh"

int main( int argc, char ** argv )
{
   using grammar = pegtl::seq< pegtl::json::text, pegtl::must< pegtl::eof > >;

   // Parse each command line argument as JSON value.

   for ( int i = 1; i < argc; ++i ) {
      if ( ! pegtl::parse< grammar, pegtl::nothing, examples::normal >( i, argv ) ) {
         std::cerr << "argv[" << i << "] is no valid JSON" << std::endl;
         return 1;
      }
   }
   return 0;
}
