// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/http_chunk_size.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test_header()
   {
      // TODO.
   }

   void test_chunked()
   {
      using GRAMMAR = http::chunked_body;

      std::size_t size = 0;
      string_input in( "0\r\n\r\n\r\n", __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR >( in, size ) );
      //      verify_rule< GRAMMAR >( __LINE__, __FILE__, "0\r\n\r\n\r\n", result_type::success );
   }

   void unit_test()
   {
      test_header();
      test_chunked();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
