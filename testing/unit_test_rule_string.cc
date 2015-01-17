// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< string<> >( "", true, 0, __LINE__ );
      verify< string< 'a', 'b' > >( "", false, 0, __LINE__ );
      verify< string< 'a', 'b' > >( "a", false, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "b", false, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "c", false, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "aa", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "aB", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "AB", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "Ab", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "ac", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "ba", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "bb", false, 2, __LINE__ );
      verify< string< 'a', 'b' > >( "aab", false, 3, __LINE__ );
      verify< string< 'a', 'b' > >( "aab", false, 3, __LINE__ );
      verify< string< 'a', 'b' > >( "ab", true, 0, __LINE__ );
      verify< string< 'a', 'b' > >( "aba", true, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "abb", true, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "abc", true, 1, __LINE__ );
      verify< string< 'a', 'b' > >( "abab", true, 2, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
