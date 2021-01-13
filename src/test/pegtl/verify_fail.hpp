// Copyright (c) 2015-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_FAIL_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_FAIL_HPP

#include <cstddef>
#include <string>

#include <tao/pegtl/memory_input.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/parse.hpp>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, template< typename > class Action = nothing, typename... States >
   void verify_fail( const std::size_t line, const char* file, const std::string& s, States&&... st )
   {
      memory_input in( s, "expect exception" );
#if defined( __cpp_exceptions )
      try {
         parse< Rule, Action >( in, st... );
         TAO_PEGTL_TEST_FAILED( "expected exception" );  // LCOV_EXCL_LINE
      }
      catch( ... ) {
      }
#else
      if( parse< Rule, Action >( in, st... ) ) {
         TAO_PEGTL_TEST_FAILED( "expected local failure" );  // LCOV_EXCL_LINE
      }
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
