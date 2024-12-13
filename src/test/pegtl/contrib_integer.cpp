// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <limits>
#include <sstream>
#include <utility>

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/contrib/integer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct int_action
      : nothing< Rule >
   {};

   template<>
   struct int_action< signed_rule >
      : signed_action
   {};

   template<>
   struct int_action< unsigned_rule >
      : unsigned_action
   {};

   template< typename S >
   void test_signed( const std::string& i, const S s )
   {
      {
         S st = -123;
         text_view_input< scan::lf > in( i );
         parse< must< signed_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         S st = -123;
         text_view_input< scan::lf > in( i );
         parse< must< signed_rule_with_action, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         text_view_input< scan::lf > in( i );
         parse< must< disable< signed_rule_with_action >, eof > >( in );
      }
      {
         S st = -123;
         text_view_input< scan::lf > in( i );
         parse< must< disable< signed_rule_with_action >, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == -123 );
      }
      {
         S st = -123;
         text_view_input< scan::lf > in( i );
         parse< must< disable< signed_rule_with_action >, eof > >( in, st, in );
         TAO_PEGTL_TEST_ASSERT( st == -123 );
      }
   }

   template< typename S >
   void test_signed( const std::string& i )
   {
      S st;
      text_view_input< scan::lf > in( i );
      TAO_PEGTL_TEST_THROWS( parse< must< signed_rule, eof >, int_action >( in, st ) );
   }

   template< typename S >
   [[nodiscard]] std::string lexical_cast( const S s )
   {
      std::ostringstream oss;
      oss << s;
      return std::move( oss ).str();
   }

   template< typename S >
   void test_signed( const S s )
   {
      S st;
      const auto i = lexical_cast( s );
      text_view_input< scan::lf > in( i );
      parse< must< signed_rule, eof >, int_action >( in, st );
      TAO_PEGTL_TEST_ASSERT( st == s );
   }

   template< typename S >
   void test_unsigned( const std::string& i, const S s )
   {
      {
         S st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule_with_action, eof >, nothing, normal, apply_mode::nothing >( in );
      }
      {
         S st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule_with_action, eof >, nothing, normal, apply_mode::nothing >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == 123 );
      }
      {
         S st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule_with_action, eof >, nothing, normal, apply_mode::nothing >( in, st, in );
         TAO_PEGTL_TEST_ASSERT( st == 123 );
      }
      {
         S st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule_with_action, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
   }

   template< typename S >
   void test_unsigned( const std::string& i )
   {
      S st = 123;
      text_view_input< scan::lf > in( i );
      TAO_PEGTL_TEST_THROWS( parse< must< unsigned_rule, eof >, int_action >( in, st ) );
   }

   template< typename S >
   void test_unsigned( const S s )
   {
      S st = 123;
      const auto i = lexical_cast( s );
      text_view_input< scan::lf > in( i );
      parse< must< unsigned_rule, eof >, int_action >( in, st );
      TAO_PEGTL_TEST_ASSERT( st == s );
   }

   void unit_test()
   {
      test_signed< signed char >( "" );
      test_signed< signed char >( "-" );
      test_signed< signed char >( "+" );
      test_signed< signed char >( "a" );

      test_signed< signed char >( "--0" );
      test_signed< signed char >( "++0" );
      test_signed< signed char >( "-+0" );

      test_signed< signed char >( "0", 0 );
      test_signed< signed char >( "+0", 0 );
      test_signed< signed char >( "-0", 0 );
      test_signed< signed char >( "000" );

      test_signed< signed char >( "+000" );
      test_signed< signed char >( "-000" );

      test_signed< signed char >( "127", 127 );
      test_signed< signed char >( "0127" );

      test_signed< signed char >( "-1", -1 );
      test_signed< signed char >( "-01" );
      test_signed< signed char >( "-001" );

      test_signed< signed char >( "-127", -127 );
      test_signed< signed char >( "-128", -128 );

      test_signed< signed char >( "128" );
      test_signed< signed char >( "-129" );
      test_signed< signed char >( "0128" );
      test_signed< signed char >( "00128" );
      test_signed< signed char >( "-0129" );
      test_signed< signed char >( "-00129" );

      test_unsigned< unsigned char >( "" );
      test_unsigned< unsigned char >( "-" );
      test_unsigned< unsigned char >( "+" );
      test_unsigned< unsigned char >( "a" );

      test_unsigned< unsigned char >( "-0" );
      test_unsigned< unsigned char >( "+1" );

      test_unsigned< unsigned char >( "0", 0 );
      test_unsigned< unsigned char >( "000" );
      test_unsigned< unsigned char >( "0", 0 );
      test_unsigned< unsigned char >( "255", 255 );
      test_unsigned< unsigned char >( "0255" );
      test_unsigned< unsigned char >( "000255" );
      test_unsigned< unsigned char >( "256" );
      test_unsigned< unsigned char >( "0256" );
      test_unsigned< unsigned char >( "000256" );

      test_signed< signed long long >( "0", 0 );
      test_signed< signed long long >( ( std::numeric_limits< signed long long >::max )() );
      test_signed< signed long long >( ( std::numeric_limits< signed long long >::min )() );

      test_unsigned< unsigned long long >( "0", 0 );
      test_unsigned< unsigned long long >( ( std::numeric_limits< unsigned long long >::max )() );

      verify_analyze< unsigned_rule >( __LINE__, __FILE__, true, false );
      verify_analyze< unsigned_rule_with_action >( __LINE__, __FILE__, true, false );

      verify_analyze< signed_rule >( __LINE__, __FILE__, true, false );
      verify_analyze< signed_rule_with_action >( __LINE__, __FILE__, true, false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
