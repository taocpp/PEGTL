// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <stdexcept>

#include "test.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct custom_error
   {};

   struct throw_std
      : one< 's' >
   {};

   struct throw_custom
      : one< 'c' >
   {};

   template< typename Rule >
   struct throwing_action
      : nothing< Rule >
   {};

   template<>
   struct throwing_action< throw_std >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/ )
      {
         throw std::runtime_error( "std" );
      }
   };

   template<>
   struct throwing_action< throw_custom >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/ )
      {
         throw custom_error();
      }
   };

   template< typename... Rules >
   using test_try_catch_rule = try_catch_return_false< must< Rules... > >;

   template< typename... Rules >
   using test_try_catch_any_rule = try_catch_any_return_false< must< Rules... > >;

   template< typename... Rules >
   using test_try_catch_type_rule = try_catch_type_return_false< parse_error_base, Rules... >;

   void test_std()
   {
      {
         text_view_input< scan::lf > in( "s" );
         TAO_PEGTL_TEST_ASSERT( !parse< try_catch_std_return_false< throw_std >, throwing_action >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
      }
      {
         text_view_input< scan::lf > in( "c" );
         TAO_PEGTL_TEST_THROWS( (void)parse< try_catch_std_return_false< throw_custom >, throwing_action >( in ) );
      }
   }

   void test_type()
   {
      {
         text_view_input< scan::lf > in( "c" );
         TAO_PEGTL_TEST_ASSERT( !parse< try_catch_type_return_false< custom_error, throw_custom >, throwing_action >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
      }
      {
         text_view_input< scan::lf > in( "s" );
         TAO_PEGTL_TEST_THROWS( (void)parse< try_catch_type_return_false< custom_error, throw_std >, throwing_action >( in ) );
      }
   }

   void unit_test()
   {
      verify_seqs< test_try_catch_rule >();
      verify_seqs< try_catch_return_false >();

      verify_seqs< test_try_catch_any_rule >();
      verify_seqs< try_catch_any_return_false >();

      verify_seqs< try_catch_std_return_false >();
      verify_seqs< test_try_catch_type_rule >();

      test_std();
      test_type();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
