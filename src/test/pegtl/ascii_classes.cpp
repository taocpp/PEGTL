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

      verify_analyze< ascii::alnum >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::alpha >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::blank >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::cntrl >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::digit >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::graph >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::identifier_first >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::identifier_other >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::lower >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::print >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::space >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::upper >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::xdigit >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::alnum >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::alpha >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::blank >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::cntrl >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::digit >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::graph >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::identifier_first >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::identifier_other >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::lower >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::print >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::space >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::upper >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::xdigit >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

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

         verify_char< ascii::alnum >( __LINE__, __FILE__, c, is_lower || is_upper || is_digit );
         verify_char< ascii::alpha >( __LINE__, __FILE__, c, is_lower || is_upper );
         verify_char< ascii::blank >( __LINE__, __FILE__, c, is_blank );
         verify_char< ascii::cntrl >( __LINE__, __FILE__, c, is_cntrl );
         verify_char< ascii::digit >( __LINE__, __FILE__, c, is_digit );
         verify_char< ascii::odigit >( __LINE__, __FILE__, c, is_odigit );
         verify_char< ascii::graph >( __LINE__, __FILE__, c, is_graph );
         verify_char< ascii::identifier_first >( __LINE__, __FILE__, c, is_ident_first );
         verify_char< ascii::identifier_other >( __LINE__, __FILE__, c, is_ident_other );
         verify_char< ascii::lower >( __LINE__, __FILE__, c, is_lower );
         verify_char< ascii::print >( __LINE__, __FILE__, c, is_print );
         verify_char< ascii::space >( __LINE__, __FILE__, c, is_space );
         verify_char< ascii::upper >( __LINE__, __FILE__, c, is_upper );
         verify_char< ascii::xdigit >( __LINE__, __FILE__, c, is_xdigit );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii::alnum >( __LINE__, __FILE__, c, false );
            verify_char< ascii::alpha >( __LINE__, __FILE__, c, false );
            verify_char< ascii::blank >( __LINE__, __FILE__, c, false );
            verify_char< ascii::cntrl >( __LINE__, __FILE__, c, false );
            verify_char< ascii::digit >( __LINE__, __FILE__, c, false );
            verify_char< ascii::odigit >( __LINE__, __FILE__, c, false );
            verify_char< ascii::graph >( __LINE__, __FILE__, c, false );
            verify_char< ascii::identifier_first >( __LINE__, __FILE__, c, false );
            verify_char< ascii::identifier_other >( __LINE__, __FILE__, c, false );
            verify_char< ascii::lower >( __LINE__, __FILE__, c, false );
            verify_char< ascii::print >( __LINE__, __FILE__, c, false );
            verify_char< ascii::space >( __LINE__, __FILE__, c, false );
            verify_char< ascii::upper >( __LINE__, __FILE__, c, false );
            verify_char< ascii::xdigit >( __LINE__, __FILE__, c, false );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii::alnum >( __LINE__, __FILE__, c, false );
            verify_char< ascii::alpha >( __LINE__, __FILE__, c, false );
            verify_char< ascii::blank >( __LINE__, __FILE__, c, false );
            verify_char< ascii::cntrl >( __LINE__, __FILE__, c, false );
            verify_char< ascii::digit >( __LINE__, __FILE__, c, false );
            verify_char< ascii::odigit >( __LINE__, __FILE__, c, false );
            verify_char< ascii::graph >( __LINE__, __FILE__, c, false );
            verify_char< ascii::identifier_first >( __LINE__, __FILE__, c, false );
            verify_char< ascii::identifier_other >( __LINE__, __FILE__, c, false );
            verify_char< ascii::lower >( __LINE__, __FILE__, c, false );
            verify_char< ascii::print >( __LINE__, __FILE__, c, false );
            verify_char< ascii::space >( __LINE__, __FILE__, c, false );
            verify_char< ascii::upper >( __LINE__, __FILE__, c, false );
            verify_char< ascii::xdigit >( __LINE__, __FILE__, c, false );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
