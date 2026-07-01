// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ascii::ranges< 'a' >, internal::one< internal::peek_char, 'a' > >();
      verify_meta< ascii::ranges< 'a', 'a' >, internal::one< internal::peek_char, 'a' > >();
      verify_meta< ascii::ranges< 'a', 'z' >, internal::range< internal::peek_char, 'a', 'z' > >();
      verify_meta< ascii::ranges< 'a', 'z', '4' >, internal::ranges< internal::peek_char, 'a', 'z', '4' > >();
      verify_meta< ascii::ranges< 'a', 'f', '0', '3' >, internal::ranges< internal::peek_char, 'a', 'f', '0', '3' > >();

      verify_ctrl_enabled< ascii::ranges< 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< ascii::ranges< 'a', 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< ascii::ranges< 'a', 'z' > >( __LINE__, __FILE__, "m" );
      verify_ctrl_disabled< internal::range< internal::peek_char, 'a', 'z' > >( __LINE__, __FILE__, "m" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_char, 'a', 'z' >, internal::peek_char > >( __LINE__, __FILE__, "m" );
      verify_ctrl_enabled< ascii::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, "4" );
      verify_ctrl_disabled< internal::ranges< internal::peek_char, 'a', 'z', '4' > >( __LINE__, __FILE__, "4" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_char, 'a', 'z', '4' >, internal::peek_char > >( __LINE__, __FILE__, "4" );
      verify_ctrl_enabled< ascii::ranges< 'a', 'f', '0', '3' > >( __LINE__, __FILE__, "2" );
      verify_ctrl_disabled< internal::ranges< internal::peek_char, 'a', 'f', '0', '3' > >( __LINE__, __FILE__, "2" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_char, 'a', 'f', '0', '3' >, internal::peek_char > >( __LINE__, __FILE__, "2" );

      verify_analyze< ascii::ranges< 'a', 'z' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::ranges< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::ranges< 'a', 'z', '4' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         const bool in_range = ( 20 <= c ) && ( c <= 120 );
         const bool in_ranges = in_range || ( c == 3 );

         verify_char< ascii::ranges< 20, 120 > >( __LINE__, __FILE__, c, in_range );
         verify_char< ascii::ranges< 20, 120, 3 > >( __LINE__, __FILE__, c, in_ranges );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
