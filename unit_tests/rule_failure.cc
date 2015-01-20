// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< failure >( "", false, 0, __LINE__ );

      for ( char i = 1; i < 127; ++i ) {
         verify_char< failure >( i, false, __LINE__ );
      }
   }

} // pegtl

#include "main.hh"
