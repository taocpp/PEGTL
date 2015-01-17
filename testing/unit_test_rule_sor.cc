// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< sor< one< 'a' >, one< 'b' > > >( "", false, 0, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "a", true, 0, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "b", true, 0, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "c", false, 1, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "aa", true, 1, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "ab", true, 1, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "ba", true, 1, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "bb", true, 1, __LINE__ );
      verify< sor< one< 'a' >, one< 'b' > > >( "cb", false, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
