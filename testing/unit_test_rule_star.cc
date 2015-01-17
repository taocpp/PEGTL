// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< star< one< 'a' > > >( "", true, 0, __LINE__ );
      verify< star< one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< star< one< 'a' > > >( "aa", true, 0, __LINE__ );
      verify< star< one< 'a' > > >( "aaa", true, 0, __LINE__ );
      verify< star< one< 'a' > > >( "ba", true, 2, __LINE__ );
      verify< star< one< 'a' > > >( "b", true, 1, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "", true, 0, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "a", true, 1, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "abb", true, 1, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "abab", true, 0, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "abab", true, 0, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "ababc", true, 1, __LINE__ );
      verify< star< one< 'a' >, one< 'b' > > >( "ababab", true, 0, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
