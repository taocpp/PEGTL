// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

#include "verify_seqs.hh"

namespace pegtl
{
   namespace test1
   {
      struct a
      {
         template< typename Input >
         static void apply( Input & in, std::string & r, std::string & s )
         {
            TEST_ASSERT( r.empty() );
            TEST_ASSERT( s.empty() );
            r += in.string();
         }
      };

      struct b
      {
         template< typename Input >
         static void apply( Input & in, std::string & r, std::string & s )
         {
            TEST_ASSERT( s.empty() );
            s += in.string();
            s += "*";
            s += r;
         }
      };

   } // namespace test1

   template< typename ... Rules >
   using if_apply_seq = if_apply< seq< Rules ... > >;

   void unit_test()
   {
      std::string r;
      std::string s;
      parse_string< must< if_apply< one< '-' >, test1::a, test1::b > > >( "-", __FILE__, r, s );
      TEST_ASSERT( r == "-" );
      TEST_ASSERT( s == "-*-" );

      verify_seqs< if_apply_seq >();
   }

} // namespace pegtl

#include "main.hh"
