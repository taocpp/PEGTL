// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cctype>
#include <clocale>

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::setlocale( LC_ALL, "C" );

      verify_analyze< ascii7::alnum >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::alpha >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::any >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::blank >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::cntrl >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::digit >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::graph >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::identifier_first >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::identifier_other >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::lower >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::print >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::space >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::upper >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::xdigit >( __LINE__, __FILE__, true, false );

      verify_analyze< ascii7::not_one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::not_one< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::not_range< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::one< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::range< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::ranges< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii7::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, true, false );

      verify_rule< ascii7::alnum >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::alpha >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::blank >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::cntrl >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::digit >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::graph >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::identifier_first >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::identifier_other >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::lower >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::print >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::space >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::upper >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::xdigit >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      verify_rule< ascii7::not_one< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::not_one< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::not_range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::one< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::one< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::ranges< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii7::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const int c = char( i );

         const bool is_blank = ( c == ' ' ) || ( c == '\t' );
         const bool is_digit = ( '0' <= c ) && ( c <= '9' );
         const bool is_odigit = ( '0' <= c ) && ( c <= '7' );
         const bool is_cntrl = ( ( 0 <= c ) && ( c <= 31 ) ) || ( c == 127 );
         const bool is_lower = ( 'a' <= c ) && ( c <= 'z' );
         const bool is_print = ( ( ' ' <= c ) && ( c <= 126 ) );
         const bool is_graph = ( ( ' ' < c ) && ( c <= 126 ) );
         const bool is_space = is_blank || ( c == '\n' ) || ( c == '\r' ) || ( c == '\v' ) || ( c == '\f' );
         const bool is_upper = ( 'A' <= c ) && ( c <= 'Z' );
         const bool is_xalpha = ( ( 'a' <= c ) && ( c <= 'f' ) ) || ( ( 'A' <= c ) && ( c <= 'F' ) );
         const bool is_xdigit = is_xalpha || is_digit;

         TAO_PEGTL_TEST_ASSERT( is_blank == bool( std::isblank( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_digit == bool( std::isdigit( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_cntrl == bool( std::iscntrl( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_lower == bool( std::islower( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_print == bool( std::isprint( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_graph == bool( std::isgraph( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_space == bool( std::isspace( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_upper == bool( std::isupper( c ) ) );
         TAO_PEGTL_TEST_ASSERT( is_xdigit == bool( std::isxdigit( c ) ) );

         const bool is_ident_first = ( c == '_' ) || is_lower || is_upper;
         const bool is_ident_other = is_ident_first || is_digit;

         verify_char< ascii7::alnum >( __LINE__, __FILE__, c, is_lower || is_upper || is_digit );
         verify_char< ascii7::alpha >( __LINE__, __FILE__, c, is_lower || is_upper );
         verify_char< ascii7::any >( __LINE__, __FILE__, c, true );
         verify_char< ascii7::blank >( __LINE__, __FILE__, c, is_blank );
         verify_char< ascii7::cntrl >( __LINE__, __FILE__, c, is_cntrl );
         verify_char< ascii7::digit >( __LINE__, __FILE__, c, is_digit );
         verify_char< ascii7::odigit >( __LINE__, __FILE__, c, is_odigit );
         verify_char< ascii7::graph >( __LINE__, __FILE__, c, is_graph );
         verify_char< ascii7::identifier_first >( __LINE__, __FILE__, c, is_ident_first );
         verify_char< ascii7::identifier_other >( __LINE__, __FILE__, c, is_ident_other );
         verify_char< ascii7::lower >( __LINE__, __FILE__, c, is_lower );
         verify_char< ascii7::print >( __LINE__, __FILE__, c, is_print );
         verify_char< ascii7::space >( __LINE__, __FILE__, c, is_space );
         verify_char< ascii7::upper >( __LINE__, __FILE__, c, is_upper );
         verify_char< ascii7::xdigit >( __LINE__, __FILE__, c, is_xdigit );

         const bool is_one = ( c == '#' ) || ( c == 'a' ) || ( c == ' ' );
         const bool is_range = ( 20 <= c ) && ( c <= 120 );
         const bool is_ranges = is_range || ( c == 3 );

         verify_char< ascii7::not_one< 'P' > >( __LINE__, __FILE__, c, c != 'P' );
         verify_char< ascii7::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, !is_one );
         verify_char< ascii7::not_range< 20, 120 > >( __LINE__, __FILE__, c, !is_range );
         verify_char< ascii7::one< 'T' > >( __LINE__, __FILE__, c, c == 'T' );
         verify_char< ascii7::one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, is_one );
         verify_char< ascii7::range< 20, 120 > >( __LINE__, __FILE__, c, is_range );
         verify_char< ascii7::ranges< 20, 120 > >( __LINE__, __FILE__, c, is_range );
         verify_char< ascii7::ranges< 20, 120, 3 > >( __LINE__, __FILE__, c, is_ranges );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii7::alnum >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::alpha >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::any >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::blank >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::cntrl >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::digit >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::odigit >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::graph >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::identifier_first >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::identifier_other >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::lower >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::print >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::space >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::upper >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::xdigit >( __LINE__, __FILE__, c, false );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii7::alnum >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::alpha >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::any >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::blank >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::cntrl >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::digit >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::odigit >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::graph >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::identifier_first >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::identifier_other >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::lower >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::print >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::space >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::upper >( __LINE__, __FILE__, c, false );
            verify_char< ascii7::xdigit >( __LINE__, __FILE__, c, false );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
