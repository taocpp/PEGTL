// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   namespace test1
   {
      struct a
      {
         template< typename Input >
         static void apply( Input &, int & r, int & s )
         {
            TEST_ASSERT( ! r );
            TEST_ASSERT( ! s );
            r = 1;
         }
      };

      struct b
      {
         template< typename Input >
         static void apply( Input &, int & r, int & s )
         {
            TEST_ASSERT( ! s );
            TEST_ASSERT( r == 1 );
            s = 2;
         }
      };

   } // namespace test1

   void unit_test()
   {
      int r = 0;
      int s = 0;
      parse_string< must< apply< test1::a, test1::b > > >( "", __FILE__, r, s );
      TEST_ASSERT( r == 1 );
      TEST_ASSERT( s == 2 );

      verify_analyze< apply<> >( __LINE__, __FILE__, false, false );

      verify_rule< apply<> >( __LINE__, __FILE__,  "", result_type::SUCCESS, 0 );

      for ( char i = 1; i < 127; ++i ) {
         char t[] = { i, 0 };
         verify_rule< apply<> >( __LINE__, __FILE__,  std::string( t ), result_type::SUCCESS, 1 );
      }
   }

} // namespace pegtl

#include "main.hh"
