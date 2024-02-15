// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEST_AFTER_SCAN_HPP
#define TAO_PEGTL_INTERNAL_TEST_AFTER_SCAN_HPP

#if defined( __cpp_exceptions )
#include <stdexcept>
#else
#include <cstdio>
#include <exception>
#endif

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename ScanInput >
   void test_after_scan( const ScanInput& in )
   {
      if( !in.empty() ) {
#if defined( __cpp_exceptions )
         throw std::runtime_error( "end of line scan has left over data" );
#else
         std::fputs( "runtime error: end of line scan has left over data\n", stderr );
         std::terminate();
#endif
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
