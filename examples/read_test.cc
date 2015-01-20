// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl.hh>

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      std::cout << pegtl::read_parser( argv[ 1 ] ).input().string() << std::endl;
   }
   return 0;
}
