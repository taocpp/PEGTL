// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< not_one< 'a' > >( "", false, 0, __LINE__ );
      verify< not_one< 'a' > >( "a", false, 1, __LINE__ );
      verify< not_one< 'a' > >( "b", true, 0, __LINE__ );
      verify< not_one< 'a', 'x' > >( "", false, 0, __LINE__ );
      verify< not_one< 'a', 'x' > >( "a", false, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "b", true, 0, __LINE__ );
      verify< not_one< 'a', 'x' > >( "w", true, 0, __LINE__ );
      verify< not_one< 'a', 'x' > >( "x", false, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "y", true, 0, __LINE__ );
      verify< not_one< 'a' > >( "an", false, 2, __LINE__ );
      verify< not_one< 'a' > >( "bn", true, 1, __LINE__ );
      verify< not_one< 'a' > >( "aa", false, 2, __LINE__ );
      verify< not_one< 'a' > >( "ba", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "an", false, 2, __LINE__ );
      verify< not_one< 'a', 'x' > >( "bn", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "aa", false, 2, __LINE__ );
      verify< not_one< 'a', 'x' > >( "ba", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "wn", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "xn", false, 2, __LINE__ );
      verify< not_one< 'a', 'x' > >( "yn", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "wa", true, 1, __LINE__ );
      verify< not_one< 'a', 'x' > >( "xa", false, 2, __LINE__ );
      verify< not_one< 'a', 'x' > >( "ya", true, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
