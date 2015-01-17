// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      for ( int i = -100; i < 200; ++i ) {
         const bool is_blank = ( i == ' ' ) || ( i == '\t' );
         const bool is_digit = ( '0' <= i ) && ( i <= '9' );
         const bool is_lower = ( 'a' <= i ) && ( i <= 'z' );
         const bool is_one = ( i == '#' ) || ( i == 'a' ) || ( i == ' ' );
         const bool is_print = ( ( ' ' <= i ) && ( i <= 126 ) );
         const bool is_range = ( 20 <= i ) && ( i <= 120 );
         const bool is_seven = ( ( i >= 0 ) && ( i <= 127 ) );
         const bool is_space = ( i == '\n' ) || ( i == '\r' ) || ( i == '\v' ) || ( i == '\f' );
         const bool is_upper = ( 'A' <= i ) && ( i <= 'Z' );
         const bool is_xalpha = ( ( 'a' <= i ) && ( i <= 'f' ) ) || ( ( 'A' <= i ) && ( i <= 'F' ) );

         verify_char< alnum >( i, is_lower || is_upper || is_digit, __LINE__ );
         verify_char< alpha >( i, is_lower || is_upper, __LINE__ );
         verify_char< any >( i, true, __LINE__ );
         verify_at_char< at_not_one< 'a', '#', ' ' > >( i, ! is_one, __LINE__ );
         verify_at_char< at_not_range< 20, 120 > >( i, ! is_range, __LINE__ );
         verify_at_char< at_one< 'a', '#', ' ' > >( i, is_one, __LINE__ );
         verify_at_char< at_range< 20, 120 > >( i, is_range, __LINE__ );
         verify_char< blank >( i, is_blank, __LINE__ );
         verify_char< digit >( i, is_digit, __LINE__ );
         verify_char< eol >( i, i == '\n', __LINE__ );
         verify_char< eolf >( i, i == '\n', __LINE__ );
         verify_char< lower >( i, is_lower, __LINE__ );
         verify_char< not_one< 'a', '#', ' ' > >( i, ! is_one, __LINE__ );
         verify_char< not_range< 20, 120 > >( i, ! is_range, __LINE__ );
         verify_char< nul >( i, i == 0, __LINE__ );
         verify_char< one< 'a', '#', ' ' > >( i, is_one, __LINE__ );
         verify_char< print >( i, is_print, __LINE__ );
         verify_char< range< 20, 120 > >( i, is_range, __LINE__ );
         verify_char< ranges< 20, 120 > >( i, is_range, __LINE__ );
         verify_char< seven >( i, is_seven, __LINE__ );
         verify_char< space >( i, is_blank || is_space, __LINE__ );
         verify_char< upper >( i, is_upper, __LINE__ );
         verify_char< xdigit >( i, is_digit || is_xalpha, __LINE__ );
      }
   }

} // pegtl

#include "unit_test_main.hh"
