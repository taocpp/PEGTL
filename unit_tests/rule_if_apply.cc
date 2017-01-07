// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

#include "verify_seqs.hh"

namespace pegtl
{
   namespace test1
   {
      struct action_a
      {
         template< typename Input >
         static void apply( const Input & in, std::string & r, std::string & s )
         {
            TEST_ASSERT( r.empty() );
            TEST_ASSERT( s.empty() );
            r += in.string();
         }
      };

      struct action_b
      {
         template< typename Input >
         static void apply( const Input & in, std::string & r, std::string & s )
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

   template< typename ... Rules >
   using if_apply_disable = if_apply< disable< Rules ... > >;

   void unit_test()
   {
      std::string state_r;
      std::string state_s;
      parse_string< must< if_apply< one< '-' >, test1::action_a, test1::action_b > > >( "-", __FILE__, state_r, state_s );
      TEST_ASSERT( state_r == "-" );
      TEST_ASSERT( state_s == "-*-" );
      parse_string< must< disable< if_apply< one< '-' >, test1::action_a, test1::action_b > > > >( "-", __FILE__, state_r, state_s );
      TEST_ASSERT( state_r == "-" );
      TEST_ASSERT( state_s == "-*-" );

      verify_seqs< if_apply_seq >();
      verify_seqs< if_apply_disable >();
   }

} // namespace pegtl

#include "main.hh"
