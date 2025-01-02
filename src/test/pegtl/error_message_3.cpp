// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
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

namespace test3
{
   using namespace TAO_PEGTL_NAMESPACE;

   // clang-format off
   struct a : one< 'a' > {};

   struct b : one< 'b' > {
      static constexpr const char* error_message = "test123";
   };

   struct grammar : sor< a, must< b > > {};
   // clang-format on

}  // namespace test3

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      try {
         parse< test3::grammar >( text_view_input< scan::lf >( "c" ) );
         TAO_PEGTL_TEST_UNREACHABLE;  // LCOV_EXCL_LINE
      }
      catch( const parse_error_base& e ) {
         TAO_PEGTL_TEST_ASSERT( e.message() == "test123" );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
