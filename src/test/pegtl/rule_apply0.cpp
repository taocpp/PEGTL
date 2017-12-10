// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace test1
      {
         struct action_a
         {
            static void apply0( int& r, int& s )
            {
               TAOCPP_PEGTL_TEST_ASSERT( !r );
               TAOCPP_PEGTL_TEST_ASSERT( !s );
               r += 1;
            }
         };

         struct action_b
         {
            static bool apply0( int& r, int& s )
            {
               TAOCPP_PEGTL_TEST_ASSERT( !s );
               TAOCPP_PEGTL_TEST_ASSERT( r == 1 );
               s += 2;
               return true;
            }
         };

         struct action2_a
         {
            static void apply0( bool& state_b )
            {
               TAOCPP_PEGTL_TEST_ASSERT( !state_b );
            }
         };

         struct action2_b
         {
            static bool apply0( bool& state_b )
            {
               TAOCPP_PEGTL_TEST_ASSERT( !state_b );
               state_b = true;
               return false;
            }
         };

         struct action2_c
         {
            static void apply0( bool& /*unused*/ )
            {
               TAOCPP_PEGTL_TEST_ASSERT( false );
            }
         };

      }  // namespace test1

      void unit_test()
      {
         int state_r = 0;
         int state_s = 0;
         parse< must< apply0< test1::action_a, test1::action_b > > >( memory_input<>( "", __FUNCTION__ ), state_r, state_s );
         TAOCPP_PEGTL_TEST_ASSERT( state_r == 1 );
         TAOCPP_PEGTL_TEST_ASSERT( state_s == 2 );
         parse< must< disable< apply< test1::action_a, test1::action_b > > > >( memory_input<>( "", __FUNCTION__ ), state_r, state_s );
         TAOCPP_PEGTL_TEST_ASSERT( state_r == 1 );
         TAOCPP_PEGTL_TEST_ASSERT( state_s == 2 );

         bool state_b = false;
         const bool result = parse< apply0< test1::action2_a, test1::action2_b, test1::action2_c > >( memory_input<>( "", __FUNCTION__ ), state_b );
         TAOCPP_PEGTL_TEST_ASSERT( !result );
         TAOCPP_PEGTL_TEST_ASSERT( state_b );

         verify_analyze< apply0<> >( __LINE__, __FILE__, false, false );

         verify_rule< apply0<> >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );

         for( char i = 1; i < 127; ++i ) {
            char t[] = { i, 0 };
            verify_rule< apply0<> >( __LINE__, __FILE__, std::string( t ), result_type::SUCCESS, 1 );
         }
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
