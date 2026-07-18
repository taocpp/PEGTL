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

#include <string>
#include <string_view>
#include <utility>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      int direct = 0;
      int member = 0;
      int free = 0;
      int nested_length = 0;
      int nested_value = 0;

      void set_member( const int value ) noexcept
      {
         member = value;
      }

      void set_nested( const int length, const int value ) noexcept
      {
         nested_length = length;
         nested_value = value;
      }
   };

   void set_free( test_target& target, const int value ) noexcept
   {
      target.free = value;
   }

   struct length_producer
   {
      template< typename Output >
      struct action
         : require_apply
      {
         template< typename ActionInput, typename... States >
         static void apply( const ActionInput& in, Output& out, States&&... /*unused*/ )
         {
            out.store( static_cast< int >( in.size() ) );
         }
      };
   };

   template< auto S >
   using length_to = build_to< S, length_producer >;

   template< typename Rule >
   using length_for = build_for< Rule, length_producer >;

   struct other_state
   {};

   struct direct_word
      : plus< alpha >
   {};

   struct member_word
      : plus< alpha >
   {};

   struct free_word
      : plus< alpha >
   {};

   struct nested_word
      : plus< alpha >
   {};

   struct number
      : plus< digit >
   {};

   struct nested_rule
      : seq< nested_word, one< ':' >, number, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< direct_word >
      : length_to< &test_target::direct >
   {};

   template<>
   struct test_action< member_word >
      : length_to< &test_target::set_member >
   {};

   template<>
   struct test_action< free_word >
      : length_to< &set_free >
   {};

   template<>
   struct test_action< nested_rule >
      : multi_to< &test_target::set_nested,
                  length_for< nested_word >,
                  number >
   {};

   template< typename Rule >
   void parse_word( const std::string_view text, other_state& other, test_target& target )
   {
      text_view_input< scan::lf > in( text );
      TAO_PEGTL_TEST_ASSERT( parse< Rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void unit_test()
   {
      other_state other;
      test_target target;

      parse_word< direct_word >( "alpha", other, target );
      parse_word< member_word >( "beta", other, target );
      parse_word< free_word >( "gamma", other, target );
      parse_word< nested_rule >( "delta:42", other, target );

      TAO_PEGTL_TEST_ASSERT( target.direct == 5 );
      TAO_PEGTL_TEST_ASSERT( target.member == 4 );
      TAO_PEGTL_TEST_ASSERT( target.free == 5 );
      TAO_PEGTL_TEST_ASSERT( target.nested_length == 5 );
      TAO_PEGTL_TEST_ASSERT( target.nested_value == 42 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
