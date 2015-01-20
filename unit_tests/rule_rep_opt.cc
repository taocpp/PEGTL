// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< rep_opt< 3, one< 'a' > > >( "", true, 0, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "aa", true, 0, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "b", true, 1, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "bb", true, 2, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "bbb", true, 3, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "aaa", true, 0, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "aaaa", true, 1, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "aaab", true, 1, __LINE__ );
      verify< rep_opt< 3, one< 'a' > > >( "baaab", true, 5, __LINE__ );

      verify< rep_opt< 2, string< 'a', 'a' > > >( "a", true, 1, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aa", true, 0, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aaa", true, 1, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aaaa", true, 0, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aaaaa", true, 1, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aaaaaa", true, 2, __LINE__ );
      verify< rep_opt< 2, string< 'a', 'a' > > >( "aaaaaaa", true, 3, __LINE__ );
   }

} // pegtl

#include "main.hh"
