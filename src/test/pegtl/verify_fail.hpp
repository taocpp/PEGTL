// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_FAIL_HPP  // NOLINT
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_FAIL_HPP

#include <cstddef>
#include <string>

#include <tao/pegtl/memory_input.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/parse.hpp>

#include "test.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule, template< typename > class Action = nothing, typename... States >
      void verify_fail( const std::size_t line, const char* file, const std::string& s, States&&... st )
      {
         try {
            memory_input<> in( s, "expect exception" );
            parse< Rule, Action >( in, st... );
            TAO_PEGTL_TEST_FAILED( "expected exception" );
         }
         catch( ... ) {
         }
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
