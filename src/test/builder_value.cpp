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
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct length_value
   {
      std::size_t value = 0;

      bool operator==( const length_value& ) const = default;
   };

   template<>
   struct value_traits< length_value >
   {
      template< typename ActionInput >
      [[nodiscard]] static length_value convert( const ActionInput& in ) noexcept
      {
         return { in.size() };
      }
   };

   template< typename T >
   struct typed_target
   {
      T value{};
   };

   struct token
      : plus< any >
   {};

   struct token_grammar
      : seq< token, eof >
   {};

   template< typename T, typename Rule >
   struct typed_action
      : nothing< Rule >
   {};

   template< typename T >
   struct typed_action< T, token >
      : value_to< &typed_target< T >::value >
   {};

   template< typename T >
   struct action_selector
   {
      template< typename Rule >
      using type = typed_action< T, Rule >;
   };

   template< typename T >
   void test_value( const std::string_view text, const T& expected )
   {
      typed_target< T > target;
      text_view_input< scan::lf > in( text );

      TAO_PEGTL_TEST_ASSERT( ( parse< token_grammar, action_selector< T >::template type >( in, target ) ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
      TAO_PEGTL_TEST_ASSERT( target.value == expected );
   }

   template< typename T >
   void test_error( const std::string_view text, const T& initial )
   {
      typed_target< T > target{ initial };
      text_view_input< scan::lf > in( text );

      TAO_PEGTL_TEST_THROWS( ( parse< token_grammar, action_selector< T >::template type >( in, target ) ) );
      TAO_PEGTL_TEST_ASSERT( target.value == initial );
   }

   struct word
      : plus< alpha >
   {};

   struct number
      : seq< opt< one< '-' > >, plus< digit > >
   {};

   struct nested_rule
      : seq< word, one< ':' >, number, eof >
   {};

   struct nested_target
   {
      std::string word;
      int number = 0;

      void set( std::string in_word, const int in_number )
      {
         word = std::move( in_word );
         number = in_number;
      }
   };

   template< typename Rule >
   struct nested_action
      : nothing< Rule >
   {};

   template<>
   struct nested_action< nested_rule >
      : multi_to< &nested_target::set,
                  value_for< word >,
                  value_for< number > >
   {};

   struct inner_value
      : plus< alpha >
   {};

   struct outer_value
      : seq< inner_value >
   {};

   struct outer_target
   {
      std::string outer;
      std::string inner;
   };

   template< typename Rule >
   struct outer_action
      : nothing< Rule >
   {};

   template<>
   struct outer_action< outer_value >
      : value_to< &outer_target::outer >
   {};

   template<>
   struct outer_action< inner_value >
      : value_to< &outer_target::inner >
   {};

   void unit_test()
   {
      test_value< char >( "x", 'x' );
      test_value< std::string >( "string", "string" );
      test_value< std::string_view >( "view", "view" );
      test_value< std::vector< char > >( "vector", { 'v', 'e', 'c', 't', 'o', 'r' } );
      test_value< float >( "1.5", 1.5f );
      test_value< double >( "2.25", 2.25 );
      test_value< signed char >( "-12", -12 );
      test_value< signed short >( "-123", -123 );
      test_value< signed int >( "-1234", -1234 );
      test_value< signed long >( "-12345", -12345 );
      test_value< signed long long >( "-123456", -123456 );
      test_value< unsigned char >( "12", 12 );
      test_value< unsigned short >( "123", 123 );
      test_value< unsigned int >( "1234", 1234 );
      test_value< unsigned long >( "12345", 12345 );
      test_value< unsigned long long >( "123456", 123456 );
      test_value< std::optional< int > >( "42", 42 );
      test_value< length_value >( "length", { 6 } );

      test_error< float >( "1.5x", 3.0f );
      test_error< double >( "1e99999", 3.0 );
      test_error< signed char >( "-1x", 3 );
      test_error< signed char >( "128", 3 );
      test_error< unsigned char >( "1x", 3 );
      test_error< unsigned char >( "256", 3 );

      nested_target nested;
      text_view_input< scan::lf > nested_input( "answer:-42" );

      TAO_PEGTL_TEST_ASSERT( parse< nested_rule, nested_action >( nested_input, nested ) );
      TAO_PEGTL_TEST_ASSERT( nested_input.empty() );
      TAO_PEGTL_TEST_ASSERT( nested.word == "answer" );
      TAO_PEGTL_TEST_ASSERT( nested.number == -42 );

      outer_target outer;
      text_view_input< scan::lf > outer_input( "value" );

      TAO_PEGTL_TEST_ASSERT( parse< outer_value, outer_action >( outer_input, outer ) );
      TAO_PEGTL_TEST_ASSERT( outer_input.empty() );
      TAO_PEGTL_TEST_ASSERT( outer.outer == "value" );
      TAO_PEGTL_TEST_ASSERT( outer.inner == "value" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
