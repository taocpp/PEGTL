// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< ranges< 'b', 'x' > >( "", false, 0, __LINE__ );
      verify< ranges< 'b', 'x' > >( "a", false, 1, __LINE__ );
      verify< ranges< 'b', 'x' > >( "y", false, 1, __LINE__ );
      verify< ranges< 'b', 'x' > >( "an", false, 2, __LINE__ );
      verify< ranges< 'b', 'x' > >( "ym", false, 2, __LINE__ );
      verify< ranges< 'b', 'x' > >( "b", true, 0, __LINE__ );
      verify< ranges< 'b', 'x' > >( "bb", true, 1, __LINE__ );
      verify< ranges< 'b', 'x' > >( "x", true, 0, __LINE__ );
      verify< ranges< 'b', 'x' > >( "xx", true, 1, __LINE__ );
      verify< ranges< 'b', 'x' > >( "n", true, 0, __LINE__ );
      verify< ranges< 'b', 'x' > >( "cv", true, 1, __LINE__ );

      verify< ranges< 'b', 'd', 'v', 'y' > >( "", false, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "a", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "e", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "u", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "z", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "_", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "m", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "b", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "c", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "d", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "v", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "w", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "y", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "bb", true, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "bv", true, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y' > >( "zb", false, 2, __LINE__ );

      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "", false, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "a", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "e", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "u", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "z", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "_", false, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "m", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "b", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "c", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "d", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "v", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "w", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "y", true, 0, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "bb", true, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "bv", true, 1, __LINE__ );
      verify< ranges< 'b', 'd', 'v', 'y', 'm' > >( "zb", false, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
