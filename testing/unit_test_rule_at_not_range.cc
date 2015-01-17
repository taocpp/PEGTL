// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< at_not_range< 'a', 'z' > >( "", false, 0, __LINE__ );
      verify< at_not_range< 'b', 'x' > >( "a", true, 1, __LINE__ );
      verify< at_not_range< 'b', 'x' > >( "y", true, 1, __LINE__ );
      verify< at_not_range< 'b', 'x' > >( "b", false, 1, __LINE__ );
      verify< at_not_range< 'b', 'x' > >( "x", false, 1, __LINE__ );
      verify< at_not_range< 'b', 'x' > >( "n", false, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
