// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <optional>
#include <string_view>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      std::optional< int > variable = 42;
      std::optional< int > argument = 42;
      std::optional< std::optional< int > > nested;
      std::vector< std::optional< int > > values;
      bool called = false;

      void set( std::optional< int > value ) noexcept
      {
         argument = value;
         called = true;
      }
   };

   struct number
      : plus< digit >
   {};

   struct member_rule
      : seq< opt< number >, eof >
   {};

   struct function_rule
      : seq< opt< number >, eof >
   {};

   struct element
      : seq< one< '[' >, opt< number >, one< ']' > >
   {};

   struct sequence_rule
      : seq< star< element >, eof >
   {};

   struct inner
      : opt< number >
   {};

   struct nested_rule
      : seq< one< '(' >, inner, one< ')' >, eof >
   {};

   struct failed_rule
      : seq< opt< number >, one< '!' > >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< member_rule >
      : optional_to< &test_target::variable, number >
   {};

   template<>
   struct test_action< function_rule >
      : optional_to< &test_target::set, number >
   {};

   template<>
   struct test_action< sequence_rule >
      : repeat_to< &test_target::values,
                   optional_for< element, number > >
   {};

   template<>
   struct test_action< nested_rule >
      : optional_to< &test_target::nested,
                     optional_for< inner, number > >
   {};

   template<>
   struct test_action< failed_rule >
      : optional_to< &test_target::variable, number >
   {};

   template< typename Rule >
   void parse_into( const std::string_view text, test_target& target )
   {
      text_view_input< scan::lf > in( text );
      TAO_PEGTL_TEST_ASSERT( parse< Rule, test_action >( in, target ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void unit_test()
   {
      test_target present;
      parse_into< member_rule >( "7", present );
      TAO_PEGTL_TEST_ASSERT( present.variable == 7 );

      test_target absent;
      parse_into< member_rule >( "", absent );
      TAO_PEGTL_TEST_ASSERT( !absent.variable );

      test_target function_present;
      parse_into< function_rule >( "8", function_present );
      TAO_PEGTL_TEST_ASSERT( function_present.called );
      TAO_PEGTL_TEST_ASSERT( function_present.argument == 8 );

      test_target function_absent;
      parse_into< function_rule >( "", function_absent );
      TAO_PEGTL_TEST_ASSERT( function_absent.called );
      TAO_PEGTL_TEST_ASSERT( !function_absent.argument );

      test_target repeated;
      parse_into< sequence_rule >( "[1][][23]", repeated );
      TAO_PEGTL_TEST_ASSERT( repeated.values == std::vector< std::optional< int > >( { 1, std::nullopt, 23 } ) );

      test_target nested_empty;
      parse_into< nested_rule >( "()", nested_empty );
      TAO_PEGTL_TEST_ASSERT( nested_empty.nested.has_value() );
      TAO_PEGTL_TEST_ASSERT( !*nested_empty.nested );

      test_target failed;
      text_view_input< scan::lf > failed_input( "7" );
      TAO_PEGTL_TEST_ASSERT( !parse< failed_rule, test_action >( failed_input, failed ) );
      TAO_PEGTL_TEST_ASSERT( failed.variable == 42 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
