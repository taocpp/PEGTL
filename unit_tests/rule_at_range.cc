// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< at_range< 'a', 'z' > >( "", false, 0, __LINE__ );
      verify< at_range< 'b', 'x' > >( "a", false, 1, __LINE__ );
      verify< at_range< 'b', 'x' > >( "y", false, 1, __LINE__ );
      verify< at_range< 'b', 'x' > >( "b", true, 1, __LINE__ );
      verify< at_range< 'b', 'x' > >( "x", true, 1, __LINE__ );
      verify< at_range< 'b', 'x' > >( "m", true, 1, __LINE__ );
   }

} // pegtl

#include "main.hh"
