// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< not_at< eof > >( "", false, 0, __LINE__ );
      verify< not_at< eof > >( " ", true, 1, __LINE__ );
      verify< not_at< any > >( "", true, 0, __LINE__ );
      verify< not_at< any > >( "a", false, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
