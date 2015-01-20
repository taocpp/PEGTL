// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< istring<> >( "", true, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "", false, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "a", false, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "b", false, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "c", false, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "aa", false, 2, __LINE__ );
      verify< istring< 'a', 'b' > >( "aB", true, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "AB", true, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "Ab", true, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "ac", false, 2, __LINE__ );
      verify< istring< 'a', 'b' > >( "ba", false, 2, __LINE__ );
      verify< istring< 'a', 'b' > >( "bb", false, 2, __LINE__ );
      verify< istring< 'a', 'b' > >( "aab", false, 3, __LINE__ );
      verify< istring< 'a', 'b' > >( "aab", false, 3, __LINE__ );
      verify< istring< 'a', 'b' > >( "ab", true, 0, __LINE__ );
      verify< istring< 'a', 'b' > >( "aba", true, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "Abb", true, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "abc", true, 1, __LINE__ );
      verify< istring< 'a', 'b' > >( "aBab", true, 2, __LINE__ );
   }

} // pegtl

#include "main.hh"
