// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< bytes< 0 > >( "", true, 0, __LINE__ );
      verify< bytes< 0 > >( "a", true, 1, __LINE__ );
      for ( char c = 0; c < 127; ++c ) {
         verify_char< bytes< 1 > >( c, true, __LINE__ );
      }
      verify< bytes< 3 > >( "abcd", true, 1, __LINE__ );
      verify< bytes< 4 > >( "abcd", true, 0, __LINE__ );
   }

} // pegtl

#include "main.hh"
