// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ascii::not_one7< 'a' >, internal::not_one< internal::peek_seven, 'a' > >();
      verify_meta< ascii::not_one7< 'a', 'c', 'z' >, internal::not_one< internal::peek_seven, 'a', 'c', 'z' > >();

      verify_meta< ascii::not_one< 'a' >, internal::not_one< internal::peek_char, 'a' > >();
      verify_meta< ascii::not_one< 'a', 'c', 'z' >, internal::not_one< internal::peek_char, 'a', 'c', 'z' > >();

      verify_analyze< ascii::not_one7< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one7< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      verify_analyze< ascii::not_one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_one< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::not_one7< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one7< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one< 'a' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_one< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const auto c = char( i );
         const bool is_one = ( c == '#' ) || ( c == 'a' ) || ( c == ' ' );

         verify_char< ascii::not_one7< 'a' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_one7< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'c' ) && ( c != 'z' ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, c != 'P' );
         verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, !is_one );

         verify_char< ascii::not_one< 'a' > >( __LINE__, __FILE__, c, c != 'a' );
         verify_char< ascii::not_one< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'c' ) && ( c != 'z' ) );
         verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, c != 'P' );
         verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, !is_one );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii::not_one7< 'a' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );

            verify_char< ascii::not_one< 'a' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, true );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii::not_one7< 'a' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'P' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_one7< 'a', '#', ' ' > >( __LINE__, __FILE__, c, false );

            verify_char< ascii::not_one< 'a' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'P' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_one< 'a', '#', ' ' > >( __LINE__, __FILE__, c, true );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
