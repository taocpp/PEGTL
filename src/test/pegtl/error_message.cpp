// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <string>

namespace TAO_PEGTL_NAMESPACE
{
   namespace test1
   {
      // clang-format off
      struct a : one< 'a' > {};
      struct b : one< 'b' > {};
      struct foo : sor< a, b > {};
      // clang-format on

   }  // namespace test1

   // clang-format off
   template<> constexpr const char* error_message< test1::a > = "test123";
   // clang-format on

   void unit_test()
   {
      try {
         parse< test1::foo >( memory_input( "b", __FUNCTION__ ) );
         TAO_PEGTL_TEST_ASSERT( false );
      }
      catch( const parse_error& e ) {
         if( e.what() != std::string( "test123" ) ) {
            throw;
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
