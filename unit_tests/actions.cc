// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   namespace test1
   {
      struct fiz : if_must< at< one< 'a' > >, two< 'a' > > {};
      struct foo : sor< fiz, one< 'b' > > {};
      struct bar : until< eof, foo > {};

      void unit_test()
      {
         TEST_ASSERT( applied.size() == 10 );

         TEST_ASSERT( applied[ 0 ].first == internal::demangle< one< 'b' > >() );
         TEST_ASSERT( applied[ 1 ].first == internal::demangle< foo >() );
         TEST_ASSERT( applied[ 2 ].first == internal::demangle< at< one< 'a' > > >() );
         TEST_ASSERT( applied[ 3 ].first == internal::demangle< two< 'a' > >() );
         TEST_ASSERT( applied[ 4 ].first == internal::demangle< fiz >() );
         TEST_ASSERT( applied[ 5 ].first == internal::demangle< foo >() );
         TEST_ASSERT( applied[ 6 ].first == internal::demangle< one< 'b' > >() );
         TEST_ASSERT( applied[ 7 ].first == internal::demangle< foo >() );
         TEST_ASSERT( applied[ 8 ].first == internal::demangle< eof >() );
         TEST_ASSERT( applied[ 9 ].first == internal::demangle< bar >() );

         TEST_ASSERT( applied[ 0 ].second == "b" );
         TEST_ASSERT( applied[ 1 ].second == "b" );
         TEST_ASSERT( applied[ 2 ].second == "" );
         TEST_ASSERT( applied[ 3 ].second == "aa" );
         TEST_ASSERT( applied[ 4 ].second == "aa" );
         TEST_ASSERT( applied[ 5 ].second == "aa" );
         TEST_ASSERT( applied[ 6 ].second == "b" );
         TEST_ASSERT( applied[ 7 ].second == "b" );
         TEST_ASSERT( applied[ 8 ].second == "" );
         TEST_ASSERT( applied[ 9 ].second == "baab" );
      }

   } // test1

   void unit_test()
   {
      parse< disable< test1::bar >, test_action >( "baab", __FILE__ );

      TEST_ASSERT( applied.size() == 1 );

      TEST_ASSERT( applied[ 0 ].first == internal::demangle< disable< test1::bar > >() );
      TEST_ASSERT( applied[ 0 ].second == "baab" );

      applied.clear();

      parse< test1::bar, test_action >( "baab", __FILE__ );

      test1::unit_test();

      applied.clear();

      parse< enable< test_action, test1::bar > >( "baab", __FILE__ );

      test1::unit_test();
   }

} // pegtl

#include "main.hh"
