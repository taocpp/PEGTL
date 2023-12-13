// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      // verify_analyze< invert< one<> > >( __LINE__, __FILE__, true, false );
      verify_analyze< invert< one< 'a' > > >( __LINE__, __FILE__, true, false );
      verify_analyze< invert< one< 'a', 'c', 'z' > > >( __LINE__, __FILE__, true, false );

      verify_analyze< invert< not_one<> > >( __LINE__, __FILE__, true, false );
      verify_analyze< invert< not_one< 'a' > > >( __LINE__, __FILE__, true, false );
      verify_analyze< invert< not_one< 'a', 'c', 'z' > > >( __LINE__, __FILE__, true, false );

      verify_analyze< invert< range< 'a', 'f' > > >( __LINE__, __FILE__, true, false );
      verify_analyze< invert< not_range< 'a', 'f' > > >( __LINE__, __FILE__, true, false );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         // verify_char< invert< one<> > >( __LINE__, __FILE__, c, true );
         verify_char< invert< one< 'a' > > >( __LINE__, __FILE__, c, c != 'a' );
         verify_char< invert< one< 'a', 'c', 'z' > > >( __LINE__, __FILE__, c, ( c != 'a' ) && ( c != 'c' ) && ( c != 'z' ) );

         verify_char< invert< not_one<> > >( __LINE__, __FILE__, c, false );
         verify_char< invert< not_one< 'a' > > >( __LINE__, __FILE__, c, c == 'a' );
         verify_char< invert< not_one< 'a', 'c', 'z' > > >( __LINE__, __FILE__, c, ( c == 'a' ) || ( c == 'c' ) || ( c == 'z' ) );

         verify_char< invert< range< 'a', 'f' > > >( __LINE__, __FILE__, c, ( c < 'a' ) || ( 'f' < c ) );
         verify_char< invert< not_range< 'a', 'f' > > >( __LINE__, __FILE__, c, ( 'a' <= c ) && ( c <= 'f' ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
