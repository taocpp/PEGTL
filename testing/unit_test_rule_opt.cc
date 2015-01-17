// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< opt< one< 'a' > > >( "", true, 0, __LINE__ );
      verify< opt< one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< opt< one< 'a' > > >( "aa", true, 1, __LINE__ );
      verify< opt< one< 'a' > > >( "ab", true, 1, __LINE__ );
      verify< opt< one< 'a' > > >( "ba", true, 2, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "", true, 0, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "a", true, 1, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "abab", true, 2, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "bab", true, 3, __LINE__ );
      verify< opt< one< 'a' >, one< 'b' > > >( "cb", true, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
