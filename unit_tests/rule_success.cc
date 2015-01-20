// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< success >( "", true, 0, __LINE__ );

      for ( char i = 1; i < 127; ++i ) {
         char t[] = { i, 0 };
         verify< success >( std::string( t ), true, 1, __LINE__ );
      }
   }

} // pegtl

#include "main.hh"
