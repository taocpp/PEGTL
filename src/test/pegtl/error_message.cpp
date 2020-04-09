// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/raise_controller.hpp>

namespace test1
{
   using namespace TAO_PEGTL_NAMESPACE;

   // clang-format off
   struct a : one< 'a' > {};
   struct b : one< 'b' > {};
   struct grammar : sor< a, b > {};

   struct my_messages { template< typename > static constexpr const char* message = nullptr; };
   template<> constexpr const char* my_messages::message< test1::a > = "test123";

   template< typename Rule > using my_control = raise_controller< my_messages >::control< Rule >;
   // clang-format on

}  // namespace test1

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      try {
         parse< test1::grammar, nothing, test1::my_control >( memory_input( "b", __FUNCTION__ ) );
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
