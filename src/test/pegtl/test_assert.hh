// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_UNIT_TESTS_TEST_ASSERT_HH
#define TAOCPP_PEGTL_INCLUDE_UNIT_TESTS_TEST_ASSERT_HH

#include <iostream>

#define TEST_ASSERT( eXPReSSioN )                       \
   do {                                                 \
   if ( ! ( eXPReSSioN ) ) {                            \
      std::cerr << "pegtl: unit test assert [ "         \
                << ( # eXPReSSioN )                     \
                << " ] failed in line [ "               \
                << __LINE__                             \
                << " ] file [ "                         \
                << __FILE__ << " ]"                     \
                << std::endl;                           \
      ++failed;                                         \
   }  } while ( 0 )

#endif
