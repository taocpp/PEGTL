// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< at_not_one< 'a' > >( "", false, 0, __LINE__ );
      verify< at_not_one< 'a' > >( "b", true, 1, __LINE__ );
      verify< at_not_one< 'a' > >( "a", false, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
