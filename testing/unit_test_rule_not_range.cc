// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< not_range< 'b', 'x' > >( "", false, 0, __LINE__ );
      verify< not_range< 'b', 'x' > >( "a", true, 0, __LINE__ );
      verify< not_range< 'b', 'x' > >( "y", true, 0, __LINE__ );
      verify< not_range< 'b', 'x' > >( "an", true, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "ym", true, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "az", true, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "yz", true, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "b", false, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "bb", false, 2, __LINE__ );
      verify< not_range< 'b', 'x' > >( "x", false, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "xx", false, 2, __LINE__ );
      verify< not_range< 'b', 'x' > >( "n", false, 1, __LINE__ );
      verify< not_range< 'b', 'x' > >( "cv", false, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
