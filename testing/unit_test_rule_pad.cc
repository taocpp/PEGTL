// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< pad< one< 'a' >, space > >( "", false, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( " ", false, 1, __LINE__ );
      verify< pad< one< 'a' >, space > >( "  ", false, 2, __LINE__ );
      verify< pad< one< 'a' >, space > >( "b", false, 1, __LINE__ );
      verify< pad< one< 'a' >, space > >( "ba", false, 2, __LINE__ );
      verify< pad< one< 'a' >, space > >( "a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( " a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "a ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "  a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "a  ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "  a  ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "   a   ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, space > >( "aa", true, 1, __LINE__ );
      verify< pad< one< 'a' >, space > >( "a a", true, 1, __LINE__ );
      verify< pad< one< 'a' >, space > >( "  a  a ", true, 2, __LINE__ );

      verify< pad< one< 'a' >, digit, blank > >( "a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "1a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "123a", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "a ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "a   ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "123a   ", true, 0, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( " a", false, 2, __LINE__ );
      verify< pad< one< 'a' >, digit, blank > >( "a1", true, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
