// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"
#include "test_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct r
      : seq< alpha, digit >
   {};

   template< typename R >
   struct a
      : nothing< R >
   {};

   template< typename R >
   struct c
      : normal< R >
   {};

   unsigned flags = 0;

   template<>
   struct a< alpha >
   {
      static void apply0()
      {
         flags |= 1;
      }
   };

   template<>
   struct a< digit >
   {
      static void apply0()
      {
         throw 42;
      }
   };

   template<>
   struct c< r >
      : normal< r >
   {
      template< typename Input >
      static void unwind( const Input& /*unused*/ )
      {
         flags |= 2;
      }
   };

   void unit_test()
   {
      text_view_input< scan::lf > in( "a1" );
      try {
         parse< r, a, c >( in );
         TAO_PEGTL_TEST_UNREACHABLE;  // LCOV_EXCL_LINE
      }
      catch( const int& e ) {
         TAO_PEGTL_TEST_ASSERT( e == 42 );
      }
      TAO_PEGTL_TEST_ASSERT( flags == 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
