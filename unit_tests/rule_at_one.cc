// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< at_one< 'a' > >( "", false, 0, __LINE__ );
      verify< at_one< 'a' > >( "b", false, 1, __LINE__ );
      verify< at_one< 'a' > >( "a", true, 1, __LINE__ );
   }

} // pegtl

#include "main.hh"
