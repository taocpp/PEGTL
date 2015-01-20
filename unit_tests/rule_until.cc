// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< until< eof > >( "", true, 0, __LINE__ );
      verify< until< any > >( "", false, 0, __LINE__ );
      verify< until< one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< until< one< 'a' > > >( "ba", true, 0, __LINE__ );
      verify< until< one< 'a' > > >( "bba", true, 0, __LINE__ );
      verify< until< one< 'a' > > >( "bbbbbbbbbbbbbbba", true, 0, __LINE__ );
      verify< until< one< 'a' > > >( "ab", true, 1, __LINE__ );
      verify< until< one< 'a' > > >( "bab", true, 1, __LINE__ );
      verify< until< one< 'a' > > >( "bbab", true, 1, __LINE__ );
      verify< until< one< 'a' > > >( "bbbbbbbbbbbbbbbab", true, 1, __LINE__ );

      verify< until< eof, any > >( "", true, 0, __LINE__ );
      verify< until< any, any > >( "", false, 0, __LINE__ );
      verify< until< one< 'a' >, any > >( "a", true, 0, __LINE__ );
      verify< until< one< 'a' >, any > >( "ba", true, 0, __LINE__ );
      verify< until< one< 'a' >, any > >( "bba", true, 0, __LINE__ );
      verify< until< one< 'a' >, any > >( "bbbbbbbbbbbbbbba", true, 0, __LINE__ );
      verify< until< one< 'a' >, any > >( "ab", true, 1, __LINE__ );
      verify< until< one< 'a' >, any > >( "bab", true, 1, __LINE__ );
      verify< until< one< 'a' >, any > >( "bbab", true, 1, __LINE__ );
      verify< until< one< 'a' >, any > >( "bbbbbbbbbbbbbbbab", true, 1, __LINE__ );

      verify< until< eof, one< 'a' > > >( "", true, 0, __LINE__ );
      verify< until< eof, one< 'a' > > >( "a", true, 0, __LINE__ );
      verify< until< eof, one< 'a' > > >( "aa", true, 0, __LINE__ );
      verify< until< eof, one< 'a' > > >( "aaaaab", false, 6, __LINE__ );
      verify< until< eof, one< 'a' > > >( "baaaaa", false, 6, __LINE__ );

      verify< until< one< 'a' >, one< 'b' > > >( "", false, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "a", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "aa", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "ab", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "b", false, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bb", false, 2, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "ba", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bba", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bbbbbbbbbbbbbba", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "baa", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bbaa", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bbbbbbbbbbbbbbaa", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bab", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bbab", true, 1, __LINE__ );
      verify< until< one< 'a' >, one< 'b' > > >( "bbbbbbbbbbbbbbab", true, 1, __LINE__ );

      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "", false, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "a", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "bca", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "bcbca", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "bcbcbcbcbca", true, 0, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "babca", false, 5, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "bcbcb", false, 5, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "cbcbc", false, 5, __LINE__ );
      verify< until< one< 'a' >, one< 'b' >, one< 'c' > > >( "bcbcbc", false, 6, __LINE__ );
   }

} // pegtl

#include "main.hh"
