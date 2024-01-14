// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< one<> >( __LINE__, __FILE__, true, false );
      verify_analyze< one< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< one< 'a', 'c', 'z' > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         verify_char< one<> >( __LINE__, __FILE__, c, false );
         verify_char< one< 'a' > >( __LINE__, __FILE__, c, c == 'a' );
         verify_char< one< 'a', 'c', 'z' > >( __LINE__, __FILE__, c, ( c == 'a' ) || ( c == 'c' ) || ( c == 'z' ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
