// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< one< 'c' > >( "", false, 0, __LINE__ );
      verify< one< 'c' > >( "c", true, 0, __LINE__ );
      verify< one< 'c' > >( "a", false, 1, __LINE__ );
      verify< one< 'c' > >( "b", false, 1, __LINE__ );
      verify< one< 'c' > >( "cc", true, 1, __LINE__ );
      verify< one< 'c' > >( "bc", false, 2, __LINE__ );
      verify< one< 'a', 'b' > >( "aa", true, 1, __LINE__ );
      verify< one< 'a', 'b' > >( "ba", true, 1, __LINE__ );
      verify< one< 'a', 'b' > >( "bc", true, 1, __LINE__ );
      verify< one< 'a', 'b' > >( "c", false, 1, __LINE__ );
      verify< one< 'a', 'b' > >( "ca", false, 2, __LINE__ );
      verify< one< 'b', 'a' > >( "aa", true, 1, __LINE__ );
      verify< one< 'b', 'a' > >( "ba", true, 1, __LINE__ );
      verify< one< 'b', 'a' > >( "bc", true, 1, __LINE__ );
      verify< one< 'b', 'a' > >( "c", false, 1, __LINE__ );
      verify< one< 'b', 'a' > >( "ca", false, 2, __LINE__ );
   }

} // pegtl

#include "main.hh"
