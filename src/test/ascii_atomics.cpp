// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
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
      verify_analyze< ascii::any7 >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::any >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one7< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one7< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_range7< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_range< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::one< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::range< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::ranges< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::any7 >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one7< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one7< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_range7< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::one< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::one< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::ranges< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const auto c = char( i );

         const bool is_one = ( c == '#' ) || ( c == 'a' ) || ( c == ' ' );
         const bool is_range = ( 20 <= c ) && ( c <= 120 );
         const bool is_ranges = is_range || ( c == 3 );

         verify_char< ascii::any7 >( __LINE__, __FILE__, c, true );
         verify_char< ascii::any >( __LINE__, __FILE__, c, true );
         verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, c != 'P' );
         verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, !is_one );
         verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, c != 'P' );
         verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, !is_one );
         verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, !is_range );
         verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, !is_range );
         verify_char< ascii::one< 'T' > >( __LINE__, __FILE__, c, c == 'T' );
         verify_char< ascii::one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, is_one );
         verify_char< ascii::range< 20, 120 > >( __LINE__, __FILE__, c, is_range );
         verify_char< ascii::ranges< 20, 120 > >( __LINE__, __FILE__, c, is_range );
         verify_char< ascii::ranges< 20, 120, 3 > >( __LINE__, __FILE__, c, is_ranges );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii::any7 >( __LINE__, __FILE__, c, false );
            verify_char< ascii::any >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::one< 'T' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::range< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::ranges< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::ranges< 20, 120, 3 > >( __LINE__, __FILE__, c, false );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii::any7 >( __LINE__, __FILE__, c, false );
            verify_char< ascii::any >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::one< 'T' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::range< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::ranges< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::ranges< 20, 120, 3 > >( __LINE__, __FILE__, c, false );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
