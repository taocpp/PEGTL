// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
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
      verify_meta< ascii::not_ione7< 'a' >, internal::not_ione< internal::peek_seven, 'a' > >();
      verify_meta< ascii::not_ione7< 'a', 'c', 'z' >, internal::not_ione< internal::peek_seven, 'a', 'c', 'z' > >();

      verify_meta< ascii::not_ione< 'a' >, internal::not_ione< internal::peek_char, 'a' > >();
      verify_meta< ascii::not_ione< 'a', 'c', 'z' >, internal::not_ione< internal::peek_char, 'a', 'c', 'z' > >();

      verify_ctrl_enabled< ascii::not_ione7< 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_ione< internal::peek_seven, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_ione< internal::peek_seven, 'a' >, internal::peek_seven > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_ione7< 'a', 'c', 'z' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_ione< internal::peek_seven, 'a', 'c', 'z' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_ione< internal::peek_seven, 'a', 'c', 'z' >, internal::peek_seven > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_ione< 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_ione< internal::peek_char, 'a' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_ione< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "b" );
      verify_ctrl_enabled< ascii::not_ione< 'a', 'c', 'z' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::not_ione< internal::peek_char, 'a', 'c', 'z' > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::terminal< internal::not_ione< internal::peek_char, 'a', 'c', 'z' >, internal::peek_char > >( __LINE__, __FILE__, "b" );

      verify_analyze< ascii::not_ione7< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_ione7< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      verify_analyze< ascii::not_ione< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_ione< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         verify_char< ascii::not_ione7< '9' > >( __LINE__, __FILE__, c, ( c != '9' ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_ione7< 'a' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'A' ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_ione7< 'A' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'A' ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_ione7< 'A', 'z' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'A' ) && ( c != 'z' ) && ( c != 'Z' ) && ( ( c & 0x80 ) == 0 ) );

         verify_char< ascii::not_ione< '9' > >( __LINE__, __FILE__, c, ( c != '9' ) );
         verify_char< ascii::not_ione< 'a' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'A' ) );
         verify_char< ascii::not_ione< 'a', 'Z' > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'A' ) && ( c != 'z' ) && ( c != 'Z' ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
