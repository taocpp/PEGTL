// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ascii::not_range7< 'a', 'a' >, internal::not_one< internal::peek_seven, 'a' > >();
      verify_meta< ascii::not_range7< 'a', 'f' >, internal::not_range< internal::peek_seven, 'a', 'f' > >();

      verify_meta< ascii::not_range< 'a', 'a' >, internal::not_one< internal::peek_char, 'a' > >();
      verify_meta< ascii::not_range< 'a', 'f' >, internal::not_range< internal::peek_char, 'a', 'f' > >();

      verify_ctrl_enabled< ascii::not_range7< 'a', 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_one< internal::peek_seven, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_seven, 'a' >, internal::peek_seven > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::not_range< internal::peek_seven, 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_seven, 'a', 'f' >, internal::peek_seven > >( __LINE__, __FILE__, "g" );
      verify_ctrl_enabled< ascii::not_range< 'a', 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::not_range< internal::peek_char, 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_char, 'a', 'f' >, internal::peek_char > >( __LINE__, __FILE__, "g" );

      verify_analyze< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::not_range7< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::not_range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const auto c = char( i );
         const bool is_range = ( 20 <= c ) && ( c <= 120 );

         verify_char< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, c, ( ( c < 'a' ) || ( 'f' < c ) ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, c, ( ( c < 'a' ) || ( 'f' < c ) ) );
         verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, !is_range );
         verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, !is_range );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, true );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, c, true );
            verify_char< ascii::not_range7< 20, 120 > >( __LINE__, __FILE__, c, false );
            verify_char< ascii::not_range< 20, 120 > >( __LINE__, __FILE__, c, true );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
