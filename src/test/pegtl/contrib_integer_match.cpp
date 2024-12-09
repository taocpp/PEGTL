// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <cstdint>

#include "test.hpp"

#include <tao/pegtl/contrib/integer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void assert_unsigned( const std::string_view sv, const bool result, const std::size_t remain = 0 )
   {
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( internal::match_unsigned( in ) == result );
      TAO_PEGTL_TEST_ASSERT( ( !result ) || ( in.size() == remain ) );
   }

   void test_unsigned()
   {
      assert_unsigned( "", false );
      assert_unsigned( "a", false );
      assert_unsigned( "00", false );
      assert_unsigned( "01", false );
      assert_unsigned( " ", false );
      assert_unsigned( "+", false );
      assert_unsigned( "-", false );
      assert_unsigned( ".", false );
      assert_unsigned( "+1", false );
      assert_unsigned( "-0", false );
      assert_unsigned( ".1", false );
      assert_unsigned( "#", false );
      assert_unsigned( "@", false );
      assert_unsigned( "%", false );
      assert_unsigned( "$", false );
      assert_unsigned( "=", false );

      assert_unsigned( "42", true );
      assert_unsigned( "10000", true );
      assert_unsigned( "65535", true );
      assert_unsigned( "65536", true );
      assert_unsigned( "100000", true );

      assert_unsigned( "0", true );
      assert_unsigned( "0a", true, 1 );
      assert_unsigned( "0.0", true, 2 );
      assert_unsigned( "9e-10", true, 4 );
      assert_unsigned( "9.9E-10", true, 6 );
      assert_unsigned( "11024901249701242104610248610284610284621048610284670129460129467", true );
   }

   void assert_success_throws( const std::string_view sv, const std::uint16_t value, const std::size_t remain = 0 )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( internal::match_and_convert_unsigned_with_maximum_throws( in, result ) );
      TAO_PEGTL_TEST_ASSERT( result == value );
      TAO_PEGTL_TEST_ASSERT( in.size() == remain );
   }

   void assert_local_failure_throws( const std::string_view sv )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( !internal::match_and_convert_unsigned_with_maximum_throws( in, result ) );
   }

   void assert_global_failure_throws( const std::string_view sv )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_THROWS( (void)internal::match_and_convert_unsigned_with_maximum_throws( in, result ) );
   }

   void test_unsigned_with_maximum_throws()
   {
      assert_local_failure_throws( "" );
      assert_local_failure_throws( "a" );
      assert_local_failure_throws( "00" );
      assert_local_failure_throws( "01" );
      assert_local_failure_throws( " " );
      assert_local_failure_throws( "+" );
      assert_local_failure_throws( "-" );
      assert_local_failure_throws( "." );
      assert_local_failure_throws( "+1" );
      assert_local_failure_throws( "-0" );
      assert_local_failure_throws( ".1" );
      assert_local_failure_throws( "#" );
      assert_local_failure_throws( "@" );
      assert_local_failure_throws( "%" );
      assert_local_failure_throws( "$" );
      assert_local_failure_throws( "=" );

      assert_success_throws( "42", 42 );
      assert_success_throws( "10000", 10000 );
      assert_success_throws( "65535", 65535 );
      assert_global_failure_throws( "65536" );
      assert_global_failure_throws( "100000" );

      assert_success_throws( "1", 1 );
      assert_success_throws( "2a", 2, 1 );
      assert_success_throws( "3.0", 3, 2 );
      assert_success_throws( "9e-10", 9, 4 );
      assert_success_throws( "9.9E-10", 9, 6 );
      assert_global_failure_throws( "11024901249701242104610248610284610284621048610284670129460129467" );
   }

   void assert_success_nothrow( const std::string_view sv, const std::uint16_t value, const std::size_t remain = 0 )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( internal::match_and_convert_unsigned_with_maximum_nothrow( in, result ) );
      TAO_PEGTL_TEST_ASSERT( result == value );
      TAO_PEGTL_TEST_ASSERT( in.size() == remain );
   }

   void assert_failure_nothrow( const std::string_view sv )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( !internal::match_and_convert_unsigned_with_maximum_nothrow( in, result ) );
   }

   template< std::uint16_t Limit >
   void assert_limited_nothrow( const std::string_view sv, const bool outcome, const std::uint16_t value = 0 )
   {
      std::uint16_t result = 0;
      view_input<> in( sv );
      TAO_PEGTL_TEST_ASSERT( internal::match_and_convert_unsigned_with_maximum_nothrow< view_input<>, std::uint16_t, Limit >( in, result ) == outcome );
      std::cerr << outcome << " " << result << " " << value << " " << sv << std::endl;
      TAO_PEGTL_TEST_ASSERT( ( !outcome ) || ( result == value ) );
   }

   void test_unsigned_with_maximum_nothrow()
   {
      assert_failure_nothrow( "" );
      assert_failure_nothrow( "a" );
      assert_failure_nothrow( "00" );
      assert_failure_nothrow( "01" );
      assert_failure_nothrow( " " );
      assert_failure_nothrow( "+" );
      assert_failure_nothrow( "-" );
      assert_failure_nothrow( "." );
      assert_failure_nothrow( "+1" );
      assert_failure_nothrow( "-0" );
      assert_failure_nothrow( ".1" );
      assert_failure_nothrow( "#" );
      assert_failure_nothrow( "@" );
      assert_failure_nothrow( "%" );
      assert_failure_nothrow( "$" );
      assert_failure_nothrow( "=" );

      assert_success_nothrow( "42", 42 );
      assert_success_nothrow( "10000", 10000 );
      assert_success_nothrow( "65535", 65535 );
      assert_failure_nothrow( "65536" );
      assert_failure_nothrow( "100000" );

      assert_success_nothrow( "1", 1 );
      assert_success_nothrow( "2a", 2, 1 );
      assert_success_nothrow( "3.0", 3, 2 );
      assert_success_nothrow( "9e-10", 9, 4 );
      assert_success_nothrow( "9.9E-10", 9, 6 );
      assert_failure_nothrow( "11024901249701242104610248610284610284621048610284670129460129467" );

      assert_limited_nothrow< 0 >( "0", true );
      assert_limited_nothrow< 1 >( "0", true );

      assert_limited_nothrow< 1 >( "0", true, 0 );
      assert_limited_nothrow< 1 >( "1", true, 1 );

      assert_limited_nothrow< 1000 >( "0", true, 0 );
      assert_limited_nothrow< 1000 >( "999", true, 999 );
      assert_limited_nothrow< 1000 >( "1000", true, 1000 );

      assert_limited_nothrow< 0 >( "1", false );
      assert_limited_nothrow< 1000 >( "1001", false );
   }

   void unit_test()
   {
      test_unsigned();
      test_unsigned_with_maximum_throws();
      test_unsigned_with_maximum_nothrow();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
