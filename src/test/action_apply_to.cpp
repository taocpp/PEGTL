// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <string_view>

#include "test.hpp"

#include <tao/pegtl/action/apply_to.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      std::string string_value;
      std::string_view view_value;
      float float_value = 0;
      double double_value = 0;
      signed char signed_value = 0;
      unsigned char unsigned_value = 0;
   };

   struct other_state
   {};

   struct rule_impl
      : plus< not_one< ',' > >
   {};

   struct string_rule
      : rule_impl
   {};

   struct view_rule
      : rule_impl
   {};

   struct float_rule
      : rule_impl
   {};

   struct double_rule
      : rule_impl
   {};

   struct signed_rule
      : rule_impl
   {};

   struct unsigned_rule
      : rule_impl
   {};

   struct float_trailing_garbage_rule
      : rule_impl
   {};

   struct double_trailing_garbage_rule
      : rule_impl
   {};

   struct signed_trailing_garbage_rule
      : rule_impl
   {};

   struct signed_out_of_range_rule
      : rule_impl
   {};

   struct unsigned_trailing_garbage_rule
      : rule_impl
   {};

   struct unsigned_out_of_range_rule
      : rule_impl
   {};

   struct grammar
      : seq< string_rule, one< ',' >, view_rule, one< ',' >, float_rule, one< ',' >, double_rule, one< ',' >, signed_rule, one< ',' >, unsigned_rule, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< string_rule >
      : apply_to< &test_target::string_value >
   {};

   template<>
   struct test_action< view_rule >
      : apply_to< &test_target::view_value >
   {};

   template<>
   struct test_action< float_rule >
      : apply_to< &test_target::float_value >
   {};

   template<>
   struct test_action< double_rule >
      : apply_to< &test_target::double_value >
   {};

   template<>
   struct test_action< signed_rule >
      : apply_to< &test_target::signed_value >
   {};

   template<>
   struct test_action< unsigned_rule >
      : apply_to< &test_target::unsigned_value >
   {};

   template<>
   struct test_action< float_trailing_garbage_rule >
      : apply_to< &test_target::float_value >
   {};

   template<>
   struct test_action< double_trailing_garbage_rule >
      : apply_to< &test_target::double_value >
   {};

   template<>
   struct test_action< signed_trailing_garbage_rule >
      : apply_to< &test_target::signed_value >
   {};

   template<>
   struct test_action< signed_out_of_range_rule >
      : apply_to< &test_target::signed_value >
   {};

   template<>
   struct test_action< unsigned_trailing_garbage_rule >
      : apply_to< &test_target::unsigned_value >
   {};

   template<>
   struct test_action< unsigned_out_of_range_rule >
      : apply_to< &test_target::unsigned_value >
   {};

#if defined( __cpp_exceptions )

   void test_float_trailing_garbage()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "1.5x" );

      TAO_PEGTL_TEST_THROWS( parse< float_trailing_garbage_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.float_value == 0 );
   }

   void test_double_trailing_garbage()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "2.25x" );

      TAO_PEGTL_TEST_THROWS( parse< double_trailing_garbage_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.double_value == 0 );
   }

   void test_signed_trailing_garbage()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "-42x" );

      TAO_PEGTL_TEST_THROWS( parse< signed_trailing_garbage_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.signed_value == 0 );
   }

   void test_signed_out_of_range()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "999999999999" );

      TAO_PEGTL_TEST_THROWS( parse< signed_out_of_range_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.signed_value == 0 );
   }

   void test_unsigned_trailing_garbage()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "123x" );

      TAO_PEGTL_TEST_THROWS( parse< unsigned_trailing_garbage_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.unsigned_value == 0 );
   }

   void test_unsigned_out_of_range()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "999999999999" );

      TAO_PEGTL_TEST_THROWS( parse< unsigned_out_of_range_rule, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.unsigned_value == 0 );
   }

#endif

   void unit_test()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "text,view,1.5,2.25,-42,123" );

      TAO_PEGTL_TEST_ASSERT( parse< grammar, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.string_value == "text" );
      TAO_PEGTL_TEST_ASSERT( target.view_value == "view" );
      TAO_PEGTL_TEST_ASSERT( target.float_value == 1.5f );
      TAO_PEGTL_TEST_ASSERT( target.double_value == 2.25 );
      TAO_PEGTL_TEST_ASSERT( target.signed_value == -42 );
      TAO_PEGTL_TEST_ASSERT( target.unsigned_value == 123 );
      TAO_PEGTL_TEST_ASSERT( in.empty() );

#if defined( __cpp_exceptions )
      test_float_trailing_garbage();
      test_double_trailing_garbage();
      test_signed_trailing_garbage();
      test_signed_out_of_range();
      test_unsigned_trailing_garbage();
      test_unsigned_out_of_range();
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
