// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< disable< one< 'c' > > >( "", false, 0, __LINE__ );
      verify< disable< one< 'c' > > >( "c", true, 0, __LINE__ );
      verify< disable< one< 'c' > > >( "a", false, 1, __LINE__ );
      verify< disable< one< 'c' > > >( "b", false, 1, __LINE__ );
      verify< disable< one< 'c' > > >( "cc", true, 1, __LINE__ );
      verify< disable< one< 'c' > > >( "bc", false, 2, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "", false, 0, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "a", false, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "b", false, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "c", false, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "abb", true, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "abc", true, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' > > >( "abab", true, 2, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' >, one< 'c' > > >( "", false, 0, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' >, one< 'c' > > >( "a", false, 1, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' >, one< 'c' > > >( "ab", false, 2, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' >, one< 'c' > > >( "abc", true, 0, __LINE__ );
      verify< disable< one< 'a' >, one< 'b' >, one< 'c' > > >( "abcd", true, 1, __LINE__ );
   }

} // pegtl

#include "main.hh"
