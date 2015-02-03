// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< pad_opt< one< 'a' >, space > >( "", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( " ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "  ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "b", true, 1, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "ba", true, 2, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "a", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( " a", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "a ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "  a", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "  b", true, 1, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "a  ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "  a  ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "   a   ", true, 0, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "aa", true, 1, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "a a", true, 1, __LINE__ );
      verify< pad_opt< one< 'a' >, space > >( "  a  a ", true, 2, __LINE__ );
   }

} // pegtl

#include "main.hh"
