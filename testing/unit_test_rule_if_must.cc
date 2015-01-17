// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< if_must< one< 'a' >, one< 'b' > > >( "", false, 0, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "a", -1, 0, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "b", false, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "ba", false, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "ab", true, 0, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "aba", true, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "abb", true, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "abab", true, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "ac", -1, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' > > >( "acb", -1, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "", false, 0, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "b", false, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "bc", false, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "a", -1, 1, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "ab", -1, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "ac", -1, 2, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "abb", -1, 3, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "acc", -1, 3, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "acb", -1, 3, __LINE__ );
      verify< if_must< one< 'a' >, one< 'b' >, one< 'c' > > >( "abc", true, 0, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
