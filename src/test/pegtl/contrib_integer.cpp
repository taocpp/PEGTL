// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/integer.hpp>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename I >
      struct int_state
      {
         I converted = 55;
      };

      template< typename Rule >
      struct int_action
         : nothing< Rule >
      {
      };

      template<>
      struct int_action< integer::signed_rule >
         : integer::signed_action
      {
      };

      template<>
      struct int_action< integer::unsigned_rule >
         : integer::unsigned_action
      {
      };

      template< typename S >
      void test_signed( const std::string& i, const S s )
      {
         int_state< S > st;
         memory_input<> in( i, __FUNCTION__ );
         parse< must< integer::signed_rule, eof >, int_action >( in, st );
         TAOCPP_PEGTL_TEST_ASSERT( st.converted == s );
      }

      template< typename S >
      void test_signed( const std::string& i )
      {
         int_state< S > st;
         memory_input<> in( i, __FUNCTION__ );
         TAOCPP_PEGTL_TEST_THROWS( parse< must< integer::signed_rule, eof >, int_action >( in, st ) );
      }

      template< typename S >
      void test_unsigned( const std::string& i, const S s )
      {
         int_state< S > st;
         memory_input<> in( i, __FUNCTION__ );
         parse< must< integer::unsigned_rule, eof >, int_action >( in, st );
         TAOCPP_PEGTL_TEST_ASSERT( st.converted == s );
      }

      template< typename S >
      void test_unsigned( const std::string& i )
      {
         int_state< S > st;
         memory_input<> in( i, __FUNCTION__ );
         TAOCPP_PEGTL_TEST_THROWS( parse< must< integer::unsigned_rule, eof >, int_action >( in, st ) );
      }

      void unit_test()
      {
         test_signed< signed char >( "--0" );
         test_signed< signed char >( "++0" );
         test_signed< signed char >( "-+0" );

         test_signed< signed char >( "0", 0 );
         test_signed< signed char >( "+0", 0 );
         test_signed< signed char >( "-0", 0 );
         test_signed< signed char >( "000", 0 );
         test_signed< signed char >( "+000", 0 );
         test_signed< signed char >( "-000", 0 );

         test_signed< signed char >( "127", 127 );

         test_signed< signed char >( "-1", -1 );
         test_signed< signed char >( "-001", -1 );

         test_signed< signed char >( "-127", -127 );
         test_signed< signed char >( "-128", -128 );

         test_signed< signed char >( "128" );
         test_signed< signed char >( "-129" );
         test_signed< signed char >( "00128" );
         test_signed< signed char >( "-00129" );

         test_unsigned< unsigned char >( "-0" );
         test_unsigned< unsigned char >( "+1" );

         test_unsigned< unsigned char >( "0", 0 );
         test_unsigned< unsigned char >( "000", 0 );
         test_unsigned< unsigned char >( "0", 0 );
         test_unsigned< unsigned char >( "255", 255 );
         test_unsigned< unsigned char >( "000255", 255 );
         test_unsigned< unsigned char >( "256" );
         test_unsigned< unsigned char >( "000256" );

         test_signed< signed long long >( "0", 0 );
         test_unsigned< unsigned long long >( "0", 0 );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
