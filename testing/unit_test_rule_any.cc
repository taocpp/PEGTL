// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< any >( "", false, 0, __LINE__ );

      for ( char i = 1; i < 127; ++i ) {
         verify_char< any >( i, true, __LINE__ );
      }
   }

} // pegtl

#include "unit_test_main.hh"
