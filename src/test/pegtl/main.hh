// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_MAIN_HH
#define TAOCPP_PEGTL_INCLUDE_TEST_MAIN_HH

#include <cstdlib>

int main( int, char ** argv )
{
   pegtl::unit_test();

   if ( pegtl::failed ) {
      std::cerr << "pegtl: unit test " << argv[ 0 ] << " failed " << pegtl::failed << std::endl;
   }
   return ( pegtl::failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif
