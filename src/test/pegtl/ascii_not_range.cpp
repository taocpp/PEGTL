// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         verify_char< ascii::not_range7< 'a', 'f' > >( __LINE__, __FILE__, c, ( ( c < 'a' ) || ( 'f' < c ) ) && ( ( c & 0x80 ) == 0 ) );
         verify_char< ascii::not_range< 'a', 'f' > >( __LINE__, __FILE__, c, ( ( c < 'a' ) || ( 'f' < c ) ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
