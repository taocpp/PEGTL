// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( "", false, 0, __LINE__ );
      verify< if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( "c", true, 0, __LINE__ );
      verify< if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( "ab", true, 0, __LINE__ );
      verify< if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( "ac", false, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
