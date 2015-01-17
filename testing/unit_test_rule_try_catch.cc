// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< try_catch< eof > >( "", true, 0, __LINE__ );
      verify< try_catch< any > >( "a", true, 0, __LINE__ );
      verify< try_catch< must< any > > >( "", false, 0, __LINE__ );
      verify< try_catch< must< one< 'a' > > > >( "b", false, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
