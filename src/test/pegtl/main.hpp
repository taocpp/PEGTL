// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_MAIN_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_MAIN_HPP

#include <cstdlib>
#include <iostream>

int main( int /*unused*/, char** argv )
{
   TAO_PEGTL_NAMESPACE::unit_test();

   if( TAO_PEGTL_NAMESPACE::failed != 0 ) {
      std::cerr << "pegtl: unit test " << argv[ 0 ] << " failed " << TAO_PEGTL_NAMESPACE::failed << std::endl;  // LCOV_EXCL_LINE
   }
   return ( TAO_PEGTL_NAMESPACE::failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif
