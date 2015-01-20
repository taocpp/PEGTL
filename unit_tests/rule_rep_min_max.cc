// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< rep_min_max< 2, 4, one< 'a' > > >( "", false, 0, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "a", false, 1, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "aa", true, 0, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "aaa", true, 0, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "aaaa", true, 0, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "aaaaa", false, 5, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "b", false, 1, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "bb", false, 2, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "bbb", false, 3, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "bbbb", false, 4, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "bbbbb", false, 5, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "ba", false, 2, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "baa", false, 3, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "baaa", false, 4, __LINE__ );
      verify< rep_min_max< 2, 4, one< 'a' > > >( "baaaa", false, 5, __LINE__ );
   }

} // pegtl

#include "main.hh"
