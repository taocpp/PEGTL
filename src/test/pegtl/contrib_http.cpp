// Copyright (c) 2019-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/contrib/http.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test_header()
   {
      // TODO.
   }

   template< typename Rule >
   struct chunked_action
      : nothing< Rule >
   {};

   template<>
   struct chunked_action< http::chunk_ext >
   {
      static void apply0( std::string& s )
      {
         s += 'a';
      }
   };

   template<>
   struct chunked_action< http::chunk_data >
   {
      static void apply0( std::string& s )
      {
         s += 'b';
      }
   };

   void test_chunked()
   {
      using GRAMMAR = must< http::chunked_body, eof >;

      verify_analyze< GRAMMAR >( __LINE__, __FILE__, true, false );
      {
         text_view_input< ascii::crlf > in( "0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR >( in ) );
      }
      {
         std::string dummy;
         text_view_input< ascii::crlf > in( "0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR >( in, dummy ) );
      }
      {
         std::string state;
         text_view_input< ascii::crlf > in( "0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR, chunked_action >( in, state ) );
         TAO_PEGTL_TEST_ASSERT( state == "a" );
      }
      {
         std::string state;
         text_view_input< ascii::crlf > in( "\r\n\r\n" );
         TAO_PEGTL_TEST_THROWS( parse< GRAMMAR, chunked_action >( in, state ) );
      }
      {
         std::string state;
         text_view_input< ascii::crlf > in( "1\r\n" );
         TAO_PEGTL_TEST_THROWS( parse< GRAMMAR, chunked_action >( in, state ) );
      }
      {
         text_view_input< ascii::crlf > in( "01\r\nX\r\n1a\r\nabcdefghijklmnopqrstuvwxyz\r\n0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR >( in ) );
      }
      {
         std::string dummy;
         text_view_input< ascii::crlf > in( "01\r\nX\r\n1a\r\nabcdefghijklmnopqrstuvwxyz\r\n0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR >( in, dummy ) );
      }
      {
         std::string state;
         text_view_input< ascii::crlf > in( "01\r\nX\r\n1A\r\nabcdefghijklmnopqrstuvwxyz\r\n0\r\n\r\n" );
         TAO_PEGTL_TEST_ASSERT( parse< GRAMMAR, chunked_action >( in, state ) );
         TAO_PEGTL_TEST_ASSERT( state == "ababa" );
      }
   }

   void unit_test()
   {
      test_header();
      test_chunked();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
