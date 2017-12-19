// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_FAIL_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_FAIL_HPP

#include <cstddef>
#include <string>

#include <tao/pegtl.hpp>

#include "test_failed.hpp"

namespace tao
{
   namespace pegtl
   {
      template< typename Rule, template< typename > class Action = nothing, typename... States >
      void verify_fail( const std::size_t line, const char* file, const std::string& s, States&&... st )
      {
         try {
            memory_input<> in( s, "expect exception" );
            parse< Rule, Action >( in, st... );
            TAOCPP_PEGTL_TEST_FAILED( "expected exception" );
         }
         catch( ... ) {
         }
      }

   }  // namespace pegtl

}  // namespace tao

#endif
