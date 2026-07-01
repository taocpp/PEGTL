// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ascii::not_ranges< 'a' >, internal::not_one< internal::peek_char, 'a' > >();
      verify_meta< ascii::not_ranges< 'a', 'a' >, internal::not_one< internal::peek_char, 'a' > >();
      verify_meta< ascii::not_ranges< 'a', 'f' >, internal::not_range< internal::peek_char, 'a', 'f' > >();
      verify_meta< ascii::not_ranges< 'a', 'f', '0' >, internal::not_ranges< internal::peek_char, 'a', 'f', '0' > >();
      verify_meta< ascii::not_ranges< 'a', 'f', '0', '3' >, internal::not_ranges< internal::peek_char, 'a', 'f', '0', '3' > >();

      verify_ctrl_enabled< ascii::not_ranges< 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_ranges< 'a', 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_ranges< 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::not_range< internal::peek_char, 'a', 'f' > >( __LINE__, __FILE__, "g" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_char, 'a', 'f' >, internal::peek_char > >( __LINE__, __FILE__, "g" );
      verify_ctrl_enabled< ascii::not_ranges< 'a', 'f', '0' > >( __LINE__, __FILE__, "_" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_char, 'a', 'f', '0' > >( __LINE__, __FILE__, "_" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_char, 'a', 'f', '0' >, internal::peek_char > >( __LINE__, __FILE__, "_" );
      verify_ctrl_enabled< ascii::not_ranges< 'a', 'f', '0', '3' > >( __LINE__, __FILE__, "_" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_char, 'a', 'f', '0', '3' > >( __LINE__, __FILE__, "_" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_char, 'a', 'f', '0', '3' >, internal::peek_char > >( __LINE__, __FILE__, "_" );

      verify_analyze< ascii::not_ranges< 'a', 'f', '0', '3' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_ranges< 'a', 'f', '0', '3', '_' > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         const bool in_ranges = ( ( 'a' <= c ) && ( c <= 'f' ) ) || ( ( '0' <= c ) && ( c <= '3' ) );
         verify_char< ascii::not_ranges< 'a', 'f', '0', '3' > >( __LINE__, __FILE__, c, !in_ranges );
         verify_char< ascii::not_ranges< 'a', 'f', '0', '3', '_' > >( __LINE__, __FILE__, c, !in_ranges && ( c != '_' ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
