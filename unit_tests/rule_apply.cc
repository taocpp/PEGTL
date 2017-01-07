// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   namespace test1
   {
      struct action_a
      {
         template< typename Input >
         static void apply( const Input &, int & r, int & s )
         {
            TEST_ASSERT( ! r );
            TEST_ASSERT( ! s );
            r += 1;
         }
      };

      struct action_b
      {
         template< typename Input >
         static void apply( const Input &, int & r, int & s )
         {
            TEST_ASSERT( ! s );
            TEST_ASSERT( r == 1 );
            s += 2;
         }
      };

   } // namespace test1

   void unit_test()
   {
      int state_r = 0;
      int state_s = 0;
      parse_string< must< apply< test1::action_a, test1::action_b > > >( "", __FILE__, state_r, state_s );
      TEST_ASSERT( state_r == 1 );
      TEST_ASSERT( state_s == 2 );
      parse_string< must< disable< apply< test1::action_a, test1::action_b > > > >( "", __FILE__, state_r, state_s );
      TEST_ASSERT( state_r == 1 );
      TEST_ASSERT( state_s == 2 );

      verify_analyze< apply<> >( __LINE__, __FILE__, false, false );

      verify_rule< apply<> >( __LINE__, __FILE__,  "", result_type::SUCCESS, 0 );

      for ( char i = 1; i < 127; ++i ) {
         char t[] = { i, 0 };
         verify_rule< apply<> >( __LINE__, __FILE__,  std::string( t ), result_type::SUCCESS, 1 );
      }
   }

} // namespace pegtl

#include "main.hh"
