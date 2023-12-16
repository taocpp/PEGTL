// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace test1
   {
      struct state1
      {
         char c = 0;

         template< typename ParseInput >
         state1( const ParseInput& /*unused*/, std::string& /*unused*/ )
         {}

         template< typename ParseInput >
         void success( const ParseInput& /*unused*/, std::string& s ) const
         {
            s += c;
         }
      };

      struct fobble
         : sor< state< state1, alpha >, digit >
      {};

      struct fibble
         : until< eof, fobble >
      {};

      template< typename Rule >
      struct action1
      {};

      template<>
      struct action1< alpha >
      {
         template< typename ActionInput >
         static void apply( const ActionInput& in, state1& s )
         {
            TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
            s.c = in.begin()[ 0 ];
         }
      };

      void state_test()
      {
         std::string result;
         test::text_input< ascii::lf > in( "dk41sk41xk3" );
         parse< fibble, action1 >( in, result );
         TAO_PEGTL_TEST_ASSERT( result == "dkskxk" );
      }

      template< typename Rule >
      struct action0
      {};

      static int i0 = 0;

      template<>
      struct action0< alpha >
      {
         static void apply0()
         {
            ++i0;
         }
      };

      template<>
      struct action0< digit >
      {
         static void apply0( std::string& s )
         {
            s += '0';
         }
      };

      void apply0_test()
      {
         test::text_input< ascii::lf > ina( "abcdefgh" );
         parse< star< alpha >, action0 >( ina );
         TAO_PEGTL_TEST_ASSERT( i0 == 8 );
         std::string s0;
         test::text_input< ascii::lf > ind( "12345678" );
         parse< star< digit >, action0 >( ind, s0 );
         TAO_PEGTL_TEST_ASSERT( s0 == "00000000" );
      }

      const std::size_t count_byte = 12345;
      const std::size_t count_line = 42;
      const std::size_t count_column = 12;

      // const char* count_source = "count_source";

      template< typename Rule >
      struct count_action
      {
         template< typename ActionInput >
         static void apply( const ActionInput& in )
         {
            TAO_PEGTL_TEST_ASSERT( in.rewind_position().count == count_byte );
            TAO_PEGTL_TEST_ASSERT( in.rewind_position().line == count_line );
            TAO_PEGTL_TEST_ASSERT( in.rewind_position().column == count_column );
            // TAO_PEGTL_TEST_ASSERT( in.input().current_position().source == count_source );
            TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( in.begin() + 1 == in.end() );
            TAO_PEGTL_TEST_ASSERT( in.peek_char() == 'f' );
            TAO_PEGTL_TEST_ASSERT( in.peek_uint8() == static_cast< unsigned char >( 'f' ) );
         }
      };

      void count_test()
      {
         const char* foo = "f";
         test::text_input< ascii::lf > in( foo, foo + 1 );
         in.private_position().count = count_byte;
         in.private_position().line = count_line;
         in.private_position().column = count_column;
         TAO_PEGTL_TEST_ASSERT( parse< alpha, count_action >( in ) );
      }

   }  // namespace test1

   void unit_test()
   {
      test1::state_test();
      test1::apply0_test();
      test1::count_test();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
