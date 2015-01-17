// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< identifier >( "_", true, 0, __LINE__ );
      verify< identifier >( "_a", true, 0, __LINE__ );
      verify< identifier >( "_1", true, 0, __LINE__ );
      verify< identifier >( "_123", true, 0, __LINE__ );
      verify< identifier >( "_1a", true, 0, __LINE__ );
      verify< identifier >( "_a1", true, 0, __LINE__ );
      verify< identifier >( "_fro_bble", true, 0, __LINE__ );
      verify< identifier >( "f_o_o42", true, 0, __LINE__ );
      verify< identifier >( "", false, 0, __LINE__ );
      verify< identifier >( "1", false, 1, __LINE__ );
      verify< identifier >( " ", false, 1, __LINE__ );
      verify< identifier >( " _", false, 2, __LINE__ );
      verify< identifier >( " a", false, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
