// Copyright (c) 2019-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct A : one< 'a' > {};
   struct B : one< 'b' > {};
   struct AB : seq< A, B > {};
   // clang-format on

   template< typename >
   struct my_action_1
   {};

   template< typename >
   struct my_action_2
   {};

   template<>
   struct my_action_1< A >
   {
      static void apply0( int& c )
      {
         if( c != 0 ) {
            throw std::runtime_error( "fail1" );  // LCOV_EXCL_LINE
         }
         c = 1;
      }
   };

   template<>
   struct my_action_1< B >
      : change_action_and_states< my_action_2, int >
   {
      // not called because my_action_2 is active
      static void apply0( int& /*v*/ )
      {
         throw std::runtime_error( "fail2" );  // LCOV_EXCL_LINE
      }

      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, int& v, int& c )
      {
         if( v != 2 ) {
            throw std::runtime_error( "fail3" );  // LCOV_EXCL_LINE
         }
         if( c != 1 ) {
            throw std::runtime_error( "fail4" );  // LCOV_EXCL_LINE
         }
         c = 3;
      }
   };

   template<>
   struct my_action_2< A >
   {
      static void apply0( int& /*c*/ )
      {
         throw std::runtime_error( "fail5" );  // LCOV_EXCL_LINE
      }
   };

   template<>
   struct my_action_2< B >
   {
      static void apply0( int& v )
      {
         if( v != 0 ) {
            throw std::runtime_error( "fail6" );  // LCOV_EXCL_LINE
         }
         v = 2;
      }
   };

   void unit_test()
   {
      {
         memory_input in( "ab", "" );
         int c = 0;
         const auto result = parse< AB, my_action_1 >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 3 );
      }
      {
         memory_input in( "a", "" );
         int c = 0;
         const auto result = parse< AB, my_action_1 >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 1 );
      }
      {
         memory_input in( "b", "" );
         int c = 0;
         const auto result = parse< AB, my_action_1 >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 0 );
      }
      {
         memory_input in( "ab", "" );
         int c = 5;
         const auto result = parse< disable< AB >, my_action_1 >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 5 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
