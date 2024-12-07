// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< ione<> >( __LINE__, __FILE__, true, false );
      verify_analyze< ione< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< ione< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         verify_char< ione<> >( __LINE__, __FILE__, c, false );
         verify_char< ione< '9' > >( __LINE__, __FILE__, c, c == '9' );
         verify_char< ione< 'a' > >( __LINE__, __FILE__, c, ( c == 'a' ) || ( c == 'A' ) );
         verify_char< ione< 'A' > >( __LINE__, __FILE__, c, ( c == 'a' ) || ( c == 'A' ) );
         verify_char< ione< 'a', 'C' > >( __LINE__, __FILE__, c, ( c == 'a' ) || ( c == 'c' ) || ( c == 'A' ) || ( c == 'C' ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
