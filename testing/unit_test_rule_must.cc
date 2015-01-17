// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< must< one< 'c' > > >( "", -1, 0, __LINE__ );
      verify< must< one< 'c' > > >( "c", true, 0, __LINE__ );
      verify< must< one< 'c' > > >( "a", -1, 1, __LINE__ );
      verify< must< one< 'c' > > >( "b", -1, 1, __LINE__ );
      verify< must< one< 'c' > > >( "cc", true, 1, __LINE__ );
      verify< must< one< 'c' > > >( "bc", -1, 2, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "", -1, 0, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "a", -1, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "b", -1, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "c", -1, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "abb", true, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "abc", true, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' > > >( "abab", true, 2, __LINE__ );
      verify< must< one< 'a' >, one< 'b' >, one< 'c' > > >( "", -1, 0, __LINE__ );
      verify< must< one< 'a' >, one< 'b' >, one< 'c' > > >( "a", -1, 1, __LINE__ );
      verify< must< one< 'a' >, one< 'b' >, one< 'c' > > >( "ab", -1, 2, __LINE__ );
      verify< must< one< 'a' >, one< 'b' >, one< 'c' > > >( "abc", true, 0, __LINE__ );
      verify< must< one< 'a' >, one< 'b' >, one< 'c' > > >( "abcd", true, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
