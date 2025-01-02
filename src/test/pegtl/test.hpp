// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_HPP

#include <cstddef>
#include <exception>
#include <iostream>

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   std::size_t failed = 0;

}  // namespace TAO_PEGTL_NAMESPACE::test

// TODO: Use std::source_location everywhere with C++20.

#define TAO_TEST_STRINGIZE_INTERNAL( ... ) #__VA_ARGS__
#define TAO_TEST_STRINGIZE( ... ) TAO_TEST_STRINGIZE_INTERNAL( __VA_ARGS__ )

#define TAO_TEST_LINE TAO_TEST_STRINGIZE( __LINE__ )

#define TAO_PEGTL_TEST_UNWRAP( ... ) __VA_ARGS__

#define TAO_PEGTL_TEST_FAILED( MeSSaGe )                   \
   do {                                                    \
      std::cerr << "pegtl: unit test failed for [ "        \
                << TAO_PEGTL_NAMESPACE::demangle< Rule >() \
                << " ] "                                   \
                << TAO_PEGTL_TEST_UNWRAP( MeSSaGe )        \
                << " in line [ "                           \
                << line                                    \
                << " ] file [ "                            \
                << file << " ]"                            \
                << std::endl;                              \
      ++TAO_PEGTL_NAMESPACE::test::failed;                 \
   } while( false )

#define TAO_PEGTL_TEST_ASSERT( ... )               \
   do {                                            \
      if( !( __VA_ARGS__ ) ) {                     \
         std::cerr << "pegtl: unit test assert [ " \
                   << ( #__VA_ARGS__ )             \
                   << " ] failed in line [ "       \
                   << __LINE__                     \
                   << " ] file [ "                 \
                   << __FILE__ << " ]"             \
                   << std::endl;                   \
         ++TAO_PEGTL_NAMESPACE::test::failed;      \
      }                                            \
   } while( false )

#define TAO_PEGTL_TEST_THROWS( ... )                \
   do {                                             \
      try {                                         \
         __VA_ARGS__;                               \
         std::cerr << "pegtl: unit test [ "         \
                   << ( #__VA_ARGS__ )              \
                   << " ] did not throw in line [ " \
                   << __LINE__                      \
                   << " ] file [ "                  \
                   << __FILE__ << " ]"              \
                   << std::endl;                    \
         ++TAO_PEGTL_NAMESPACE::test::failed;       \
      }                                             \
      catch( ... ) {                                \
      }                                             \
   } while( false )

#define TAO_PEGTL_TEST_UNREACHABLE                                                                                              \
   do {                                                                                                                         \
      std::cerr << "Code should be unreachable in " << __FUNCTION__ << " (" << __FILE__ << ':' << __LINE__ << ')' << std::endl; \
      std::terminate();                                                                                                         \
   } while( false )

#endif
