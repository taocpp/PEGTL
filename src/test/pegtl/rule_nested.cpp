// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/member.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct token
   {
      std::string value;
      int dummy;
   };

   using inner_input = internal::input_with_funcs< internal::input_with_fakes< internal::input_with_start< internal::view_input< char > > > >;
   using outer_input = internal::input_with_fakes< internal::copy_input< std::vector< token > > >;

   using inner_grammar = seq< string< 'f', 'o', 'o' >, eof >;
   using outer_grammar = seq< member::nested< &token::value, inner_grammar >, eof >;

   void test_inner()
   {
      inner_input in( "foo" );
      TAO_PEGTL_TEST_ASSERT( in.size() == 3 );
      const auto b = parse< inner_grammar >( in );
      TAO_PEGTL_TEST_ASSERT( b );
   }

   void test_success()
   {
      outer_input in( { { "foo", 1 } } );
      TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
      const auto b = parse< outer_grammar >( in );
      TAO_PEGTL_TEST_ASSERT( b );
   }

   void test_failure1()
   {
      outer_input in( { { "bar", 1 } } );
      TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
      const auto b = parse< outer_grammar >( in );
      TAO_PEGTL_TEST_ASSERT( !b );
   }

   void test_failure2()
   {
      outer_input in( {} );
      TAO_PEGTL_TEST_ASSERT( in.size() == 0 );
      const auto b = parse< outer_grammar >( in );
      TAO_PEGTL_TEST_ASSERT( !b );
   }

   void test_failure3()
   {
      outer_input in( { { "foo", 1 }, { "foo", 1  } } );
      TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
      const auto b = parse< outer_grammar >( in );
      TAO_PEGTL_TEST_ASSERT( !b );
   }

   void unit_test()
   {
      test_inner();
      test_success();
      test_failure1();
      test_failure2();
      test_failure3();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
