// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <limits>
#include <sstream>

#include "test.hpp"

#include "verify_rule.hpp"

#include <tao/pegtl/contrib/integerx.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename I >
   struct int_state
   {
      I converted = 55;
   };

   template< typename Rule >
   struct int_action
      : nothing< Rule >
   {};

   template<>
   struct int_action< integerx::signed_rule >
      : integerx::signed_hex_action
   {};

   template<>
   struct int_action< integerx::unsigned_rule >
      : integerx::unsigned_hex_action
   {};

   template< typename S >
   void test_signed( const std::string& i, const S s )
   {
      static_assert( std::is_signed_v< S > );
      {
         S st = -123;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::signed_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         int_state< S > st;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::signed_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st.converted == s );
      }
      {
         S st = -123;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::signed_rule_with_action, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
   }

   template< typename S >
   void test_signed( const std::string& i )
   {
      int_state< S > st;
      memory_input in( i, __FUNCTION__ );
      TAO_PEGTL_TEST_THROWS( parse< must< integerx::signed_rule, eof >, int_action >( in, st ) );
   }

   template< typename S >
   std::string lexical_cast( const S s )
   {
      std::ostringstream o;
      o << s;
      return o.str();
   }

   template< typename S >
   void test_signed( const S s )
   {
      int_state< S > st;
      const auto i = lexical_cast( s );
      memory_input in( i, __FUNCTION__ );
      parse< must< integerx::signed_rule, eof >, int_action >( in, st );
      TAO_PEGTL_TEST_ASSERT( st.converted == s );
   }

   template< typename S >
   void test_unsigned( const std::string& i, const S s )
   {
      static_assert( std::is_unsigned_v< S > );
      {
         S st = 123;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::unsigned_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         int_state< S > st;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::unsigned_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st.converted == s );
      }
      {
         S st = 123;
         memory_input in( i, __FUNCTION__ );
         parse< must< integerx::unsigned_rule_with_action, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
   }

   template< typename S >
   void test_unsigned( const std::string& i )
   {
      S st = 123;
      memory_input in( i, __FUNCTION__ );
      TAO_PEGTL_TEST_THROWS( parse< must< integerx::unsigned_rule, eof >, int_action >( in, st ) );
   }

   template< typename S >
   void test_unsigned( const S s )
   {
      S st = 123;
      const auto i = lexical_cast( s );
      memory_input in( i, __FUNCTION__ );
      parse< must< integerx::unsigned_rule, eof >, int_action >( in, st );
      TAO_PEGTL_TEST_ASSERT( st == s );
   }

   template< auto M >
   using max_seq_rule = seq< one< 'a' >, integerx::maximum_rule< std::uint64_t, M >, one< 'y' >, eof >;

   void unit_test()
   {
      test_signed< signed char >( "" );
      test_signed< signed char >( "-" );
      test_signed< signed char >( "+" );
      test_signed< signed char >( "a" );
      test_signed< signed char >( "0x" );
      test_signed< signed char >( "-0x" );
      test_signed< signed char >( "+0x" );
      test_signed< signed char >( "0xh" );
      test_signed< signed char >( "-0xh" );
      test_signed< signed char >( "+0xh" );

      test_signed< signed char >( "--0" );
      test_signed< signed char >( "++0" );
      test_signed< signed char >( "-+0" );

      test_signed< signed char >( "0x0", 0 );
      test_signed< signed char >( "+0x0", 0 );
      test_signed< signed char >( "-0x0", 0 );
      test_signed< signed char >( "0x000", 0 );
      test_signed< signed char >( "000x000" );
      test_signed< signed char >( "+0x000", 0 );
      test_signed< signed char >( "-0x000", 0 );

      test_signed< signed char >( "000" );
      test_signed< signed char >( "+000" );
      test_signed< signed char >( "-000" );

      test_signed< signed char >( "0x7f", 127 );
      test_signed< signed char >( "0x007f", 127 );

      test_signed< signed char >( "-0x1", -1 );
      test_signed< signed char >( "-0x01", -1 );
      test_signed< signed char >( "-0x001", -1 );

      test_signed< signed char >( "-0x7f", -127 );
      test_signed< signed char >( "-0x80", -128 );

      test_signed< signed char >( "0x80" );
      test_signed< signed char >( "-0x81" );

      test_unsigned< unsigned char >( "" );
      test_unsigned< unsigned char >( "-" );
      test_unsigned< unsigned char >( "+" );
      test_unsigned< unsigned char >( "a" );

      test_unsigned< unsigned char >( "-0" );
      test_unsigned< unsigned char >( "+1" );

      test_unsigned< unsigned char >( "0x0", 0 );
      test_unsigned< unsigned char >( "0x0", 0 );
      test_unsigned< unsigned char >( "0x000", 0 );
      test_unsigned< unsigned char >( "0xff", 255 );
      test_unsigned< unsigned char >( "0x100" );

      test_signed< signed long long >( "0x0", 0 );
      test_signed< signed long long >( "0x7fffffffffffffff", ( std::numeric_limits< signed long long >::max )() );
      test_signed< signed long long >( "-0x8000000000000000", ( std::numeric_limits< signed long long >::min )() );

      test_unsigned< unsigned long long >( "0x0", 0 );
      test_unsigned< unsigned long long >( "0xffffffffffffffff", ( std::numeric_limits< unsigned long long >::max )() );

      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "ay", result_type::local_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a0x1y", result_type::global_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a0x9y", result_type::global_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0x1y", result_type::success );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0x2y", result_type::global_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0x9y", result_type::global_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x1y", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x2y", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x3y", result_type::global_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x9y", result_type::global_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0x3y", result_type::success );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0x4y", result_type::global_failure );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 4 > >( __LINE__, __FILE__, "a0x5y", result_type::global_failure );
      verify_rule< max_seq_rule< 4 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0x9y", result_type::success );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0x10y", result_type::global_failure );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );

      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0x9y", result_type::success );
      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0x10y", result_type::success );
      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0x11y", result_type::global_failure );
      verify_rule< max_seq_rule< 16 > >( __LINE__, __FILE__, "a0x19y", result_type::global_failure );

      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x9y", result_type::success );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x10y", result_type::success );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x11y", result_type::success );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x12y", result_type::global_failure );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x13y", result_type::global_failure );
      verify_rule< max_seq_rule< 17 > >( __LINE__, __FILE__, "a0x111y", result_type::global_failure );

      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x0y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x1y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x9y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x10y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x11y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x12y", result_type::success );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0xy", result_type::local_failure );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x13y", result_type::global_failure );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x19y", result_type::global_failure );
      verify_rule< max_seq_rule< 18 > >( __LINE__, __FILE__, "a0x111y", result_type::global_failure );

      verify_rule< max_seq_rule< 18446744073709551614ULL > >( __LINE__, __FILE__, "a0xfffffffffffffffey", result_type::success );
      verify_rule< max_seq_rule< 18446744073709551614ULL > >( __LINE__, __FILE__, "a0xffffffffffffffffy", result_type::global_failure );

      verify_rule< max_seq_rule< 18446744073709551615ULL > >( __LINE__, __FILE__, "a0xffffffffffffffffy", result_type::success );
      verify_rule< max_seq_rule< 18446744073709551615ULL > >( __LINE__, __FILE__, "a0x10000000000000000y", result_type::global_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
