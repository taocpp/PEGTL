// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< eolf >( "", true, 0, __LINE__ );
      verify< eolf >( "a", false, 1, __LINE__ );
      verify< eolf >( "\n", true, 0, __LINE__ );
      verify< eolf >( "\r", false, 1, __LINE__ );
      verify< eolf >( "\r\n", false, 2, __LINE__ );
      verify< eolf >( "\n\r", true, 1, __LINE__ );
      verify< eolf >( "\na", true, 1, __LINE__ );
      verify< eolf >( "\ra", false, 2, __LINE__ );
      verify< eolf >( "\r\na", false, 3, __LINE__ );
      verify< eolf >( "\r\n\r", false, 3, __LINE__ );
      verify< eolf >( "\r\n\n", false, 3, __LINE__ );
   }

} // pegtl

#include "main.hh"
