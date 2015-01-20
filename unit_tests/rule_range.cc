// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< range< 'b', 'x' > >( "", false, 0, __LINE__ );
      verify< range< 'b', 'x' > >( "a", false, 1, __LINE__ );
      verify< range< 'b', 'x' > >( "y", false, 1, __LINE__ );
      verify< range< 'b', 'x' > >( "an", false, 2, __LINE__ );
      verify< range< 'b', 'x' > >( "ym", false, 2, __LINE__ );
      verify< range< 'b', 'x' > >( "b", true, 0, __LINE__ );
      verify< range< 'b', 'x' > >( "bb", true, 1, __LINE__ );
      verify< range< 'b', 'x' > >( "x", true, 0, __LINE__ );
      verify< range< 'b', 'x' > >( "xx", true, 1, __LINE__ );
      verify< range< 'b', 'x' > >( "n", true, 0, __LINE__ );
      verify< range< 'b', 'x' > >( "cv", true, 1, __LINE__ );
   }

} // pegtl

#include "main.hh"
