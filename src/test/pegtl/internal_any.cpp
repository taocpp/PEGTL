// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/internal/peek_endian.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using any7 = internal::any< internal::peek_seven >;
      using any = internal::any< internal::peek_char >;
      using any32 = internal::any< internal::peek_uint32_be >;

      verify_meta< any7, internal::any< internal::peek_seven > >();
      verify_meta< any, internal::any< internal::peek_char > >();
      verify_meta< any32, internal::any< internal::peek_uint32_be > >();

      verify_analyze< any7 >( __LINE__, __FILE__, true, false );
      verify_analyze< any >( __LINE__, __FILE__, true, false );
      verify_analyze< any32 >( __LINE__, __FILE__, true, false );
      {
         char c = char( 200 );
         view_input< scan::lf > in( &c, 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< any7 >( in ) );
         TAO_PEGTL_TEST_ASSERT( !parse< any32 >( in ) );
         TAO_PEGTL_TEST_ASSERT( parse< any >( in ) );
      }
      {
         view_input< scan::lf > in( "fg" );
         TAO_PEGTL_TEST_ASSERT( parse< any7 >( in ) );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( *in.current() == 'g' );
      }
      {
         view_input< scan::lf > in( "fg" );
         TAO_PEGTL_TEST_ASSERT( parse< any >( in ) );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( *in.current() == 'g' );
      }
      {
         view_input< void > in( "abcd" );
         TAO_PEGTL_TEST_ASSERT( parse< any32 >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::uint32_t t = 91283821;
         view_input< void, std::uint32_t > in( &t, 1 );
         TAO_PEGTL_TEST_ASSERT( parse< any32 >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
