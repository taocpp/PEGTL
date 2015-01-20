// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_TESTING_UNIT_TEST_MAIN_HH
#define PEGTL_TESTING_UNIT_TEST_MAIN_HH

int main( int, char ** argv )
{
   pegtl::unit_test();
   std::cerr << "pegtl: " << argv[ 0 ] << " done [ " << pegtl::total << " ] fail [ " << pegtl::fails << " ]" << std::endl;
   return bool( pegtl::fails );
}

#endif
