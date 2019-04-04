// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
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
   struct my_action
   {};

   template<>
   struct my_action< A >
   {
      static void apply0( int& c )
      {
         if( c != 0 ) {
            throw std::runtime_error( "fail1" );
         }
         c = 1;
      }
   };

   struct S
   {
      int v = 0;

      template< typename Input >
      explicit S( const Input& /*unused*/, int& c )
      {
         if( c == 5 ) {
            v = 6;
         }
         else {
            if( c != 1 ) {
               throw std::runtime_error( "fail2" );
            }
            v = 2;
         }
      }

      template< typename Input >
      void success( const Input& /*unused*/, int& c )
      {
         if( v != 3 ) {
            throw std::runtime_error( "fail3" );
         }
         c = 4;
      }
   };

   template<>
   struct my_action< B >
      : change_state< S >
   {
      static void apply0( S& s )
      {
         if( s.v != 2 ) {
            throw std::runtime_error( "fail4" );
         }
         s.v = 3;
      }
   };

   void unit_test()
   {
      {
         memory_input in( "ab", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 4 );
      }
      {
         memory_input in( "a", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 1 );
      }
      {
         memory_input in( "b", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 0 );
      }
      {
         memory_input in( "ab", "" );
         int c = 5;
         const auto result = parse< disable< AB >, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 5 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
