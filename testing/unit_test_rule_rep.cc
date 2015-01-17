// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< rep< 3, one< 'a' > > >( "", false, 0, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "a", false, 1, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "aa", false, 2, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "b", false, 1, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "bb", false, 2, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "bbb", false, 3, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "aaa", true, 0, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "aaaa", true, 1, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "aaab", true, 1, __LINE__ );
      verify< rep< 3, one< 'a' > > >( "baaab", false, 5, __LINE__ );

      verify< rep< 2, string< 'a', 'a' > > >( "a", false, 1, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aa", false, 2, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aaa", false, 3, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aaaa", true, 0, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aaaaa", true, 1, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aaaaaa", true, 2, __LINE__ );
      verify< rep< 2, string< 'a', 'a' > > >( "aaaaaaa", true, 3, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
