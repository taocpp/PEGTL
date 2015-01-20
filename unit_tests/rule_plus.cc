// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< plus< one< 'a' > > >( "", false, 0, __LINE__ );
      verify< plus< one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< plus< one< 'a' > > >( "aa", true, 0, __LINE__ );
      verify< plus< one< 'a' > > >( "aaa", true, 0, __LINE__ );
      verify< plus< one< 'a' > > >( "b", false, 1, __LINE__ );
      verify< plus< one< 'a' > > >( "ab", true, 1, __LINE__ );
      verify< plus< one< 'a' > > >( "aab", true, 1, __LINE__ );
      verify< plus< one< 'a' > > >( "aaab", true, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "", false, 0, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "a", false, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "b", false, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "ac", false, 2, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "aa", false, 2, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "abb", true, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "abc", true, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "abab", true, 0, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "ababa", true, 1, __LINE__ );
      verify< plus< one< 'a' >, one< 'b' > > >( "ababb", true, 1, __LINE__ );
   }

} // pegtl

#include "main.hh"
