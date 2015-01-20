// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< at< eof > >( "", true, 0, __LINE__ );
      verify< at< eof > >( " ", false, 1, __LINE__ );
      verify< at< any > >( "", false, 0, __LINE__ );
      verify< at< any > >( "a", true, 1, __LINE__ );
      verify< at< any > >( "aa", true, 2, __LINE__ );
   }

} // pegtl

#include "main.hh"
