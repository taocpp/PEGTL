// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <cassert>

#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/json.hh>

using GRAMMAR = pegtl::json::text;

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << i << std::endl;
      pegtl::parse< GRAMMAR >( i, argv );
   }
   return 0;
}
