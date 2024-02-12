// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

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
         text_view_input< scan::lf > in( "dk41sk41xk3" );
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
         text_view_input< scan::lf > ina( "abcdefgh" );
         parse< star< alpha >, action0 >( ina );
         TAO_PEGTL_TEST_ASSERT( i0 == 8 );
         std::string s0;
         text_view_input< scan::lf > ind( "12345678" );
         parse< star< digit >, action0 >( ind, s0 );
         TAO_PEGTL_TEST_ASSERT( s0 == "00000000" );
      }

      std::size_t count = 0;
      text_position pos[] = { { 0, 1, 1 }, { 1, 1, 2 }, { 2, 2, 1 }, { 3, 2, 2 } };

      template< typename Rule >
      struct count_action
         : nothing< Rule >
      {};

      template<>
      struct count_action< any >
      {
         template< typename ActionInput >
         static void apply( const ActionInput& in )
         {
            TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( in.begin() + 1 == in.end() );

            if( in.peek_char() == 'a' ) {
               TAO_PEGTL_TEST_ASSERT( in.current_position() == pos[ 0 ] );
               TAO_PEGTL_TEST_ASSERT( in.input().current_position() == pos[ 1 ] );
            }
            else if( in.peek_char() == '\n' ) {
               TAO_PEGTL_TEST_ASSERT( in.current_position() == pos[ 1 ] );
               TAO_PEGTL_TEST_ASSERT( in.input().current_position() == pos[ 2 ] );
            }
            else if( in.peek_char() == 'b' ) {
               TAO_PEGTL_TEST_ASSERT( in.current_position() == pos[ 2 ] );
               TAO_PEGTL_TEST_ASSERT( in.input().current_position() == pos[ 3 ] );
            }
            else {
               TAO_PEGTL_TEST_UNREACHABLE;
            }
            ++count;
         }
      };

      void count_test()
      {
         text_view_input< scan::lf > in( "a\nb" );
         TAO_PEGTL_TEST_ASSERT( parse< plus< any >, count_action >( in ) );
         TAO_PEGTL_TEST_ASSERT( count == 3 );
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
