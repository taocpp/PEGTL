// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/example/semver2.hpp>

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   using namespace TAO_PEGTL_NAMESPACE;

   struct grammar : seq< semver2::valid_semver, eof > {};

   for( int i = 1; i < argc; ++i ) {
      argv_input in( argv, i );
      const auto b = parse< grammar >( in );
      if( b ) {
         std::cout << argv[ i ] << " is a valid semver" << std::endl;
      }
      else {
         std::cout << argv[ i ] << " is NOT a valid semver" << std::endl;
      }
   }
   return 0;
}
