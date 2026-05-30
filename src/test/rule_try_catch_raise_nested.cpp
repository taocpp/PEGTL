// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#elif defined( __clang__ ) && defined( _WIN32 )
#include <iostream>
int main()
{
   std::cout << "Nested exceptions are broken with Clang on Windows, skipping test..." << std::endl;
}
#else

#include <stdexcept>
#include <vector>

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_seqs.hpp"

#include <tao/pegtl/extra/nested_exceptions.hpp>

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
   using test_try_catch_rule = try_catch_raise_nested< must< Rules... > >;

   template< typename... Rules >
   using test_try_catch_any_rule = try_catch_any_raise_nested< must< Rules... > >;

   template< typename... Rules >
   using test_try_catch_type_rule = try_catch_type_raise_nested< parse_error_base, Rules... >;

   template< template< typename... > class Rule >
   void verify_nested()
   {
      try {
         text_view_input< scan::lf > in( "0" );
         parse< Rule< alpha > >( in );
         TAO_PEGTL_TEST_UNREACHABLE;
      }
      catch( ... ) {
         const std::vector< parse_error_base > e = flatten_base();
         TAO_PEGTL_TEST_ASSERT( e.size() == 2 );
      }
   }

   void test_std()
   {
      {
         text_view_input< scan::lf > in( "s" );
         try {
            parse< try_catch_std_raise_nested< throw_std >, throwing_action >( in );
            TAO_PEGTL_TEST_UNREACHABLE;
         }
         catch( const parse_error_base& ) {
         }
      }
      {
         text_view_input< scan::lf > in( "c" );
         try {
            parse< try_catch_std_raise_nested< throw_custom >, throwing_action >( in );
            TAO_PEGTL_TEST_UNREACHABLE;
         }
         catch( const custom_error& ) {
         }
      }
   }

   void test_type()
   {
      {
         text_view_input< scan::lf > in( "c" );
         try {
            parse< try_catch_type_raise_nested< custom_error, throw_custom >, throwing_action >( in );
            TAO_PEGTL_TEST_UNREACHABLE;
         }
         catch( const parse_error_base& ) {
         }
      }
      {
         text_view_input< scan::lf > in( "s" );
         try {
            parse< try_catch_type_raise_nested< custom_error, throw_std >, throwing_action >( in );
            TAO_PEGTL_TEST_UNREACHABLE;
         }
         catch( const std::runtime_error& ) {
         }
      }
   }

   void unit_test()
   {
      verify_seqs< try_catch_raise_nested >();
      verify_seqs< try_catch_any_raise_nested >();
      verify_seqs< try_catch_std_raise_nested >();
      verify_seqs< test_try_catch_type_rule >();

      verify_nested< test_try_catch_rule >();
      verify_nested< test_try_catch_any_rule >();

      test_std();
      test_type();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
