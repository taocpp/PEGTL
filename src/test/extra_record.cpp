// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/extra/record.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct one_a : one< 'a' > {};
   struct one_b : one< 'b' > {};
   struct one_c : one< 'c' > {};
   struct seq_ab : seq< one_a, one_b > {};
   struct seq_ac : seq< one_a, one_c > {};
   struct sor_ab_ac : sor< seq_ab, seq_ac > {};
   struct grammar : seq< sor_ab_ac, eof > {};
   // clang-format on

   template< typename Rule, typename Sample >
   void verify_sample( const Sample& sample, const std::string_view data, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      TAO_PEGTL_TEST_ASSERT( sample.rule == demangle< Rule >() );
      TAO_PEGTL_TEST_ASSERT( sample.input.string_view() == data );

      const auto& offset = sample.input.direct_offset();
      TAO_PEGTL_TEST_ASSERT( offset.line == line );
      TAO_PEGTL_TEST_ASSERT( offset.column == column );
      TAO_PEGTL_TEST_ASSERT( offset.count == count );
   }

   void test_selected_terminals()
   {
      text_view_input<> in( "ac" );
      const auto v = record< one_a, one_b, one_c >::parse< grammar >( in );
      TAO_PEGTL_TEST_ASSERT( v.size() == 2 );
      verify_sample< one_a >( v[ 0 ], "a", 1, 1, 0 );
      verify_sample< one_c >( v[ 1 ], "c", 1, 2, 1 );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void test_first_branch()
   {
      text_view_input<> in( "ab" );
      const auto v = record< one_a, one_b, one_c, seq_ab, seq_ac, sor_ab_ac >::parse< grammar >( in );
      TAO_PEGTL_TEST_ASSERT( v.size() == 4 );
      verify_sample< one_a >( v[ 0 ], "a", 1, 1, 0 );
      verify_sample< one_b >( v[ 1 ], "b", 1, 2, 1 );
      verify_sample< seq_ab >( v[ 2 ], "ab", 1, 1, 0 );
      verify_sample< sor_ab_ac >( v[ 3 ], "ab", 1, 1, 0 );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void test_second_branch()
   {
      text_view_input<> in( "ac" );
      const auto v = record< one_a, one_b, one_c, seq_ab, seq_ac, sor_ab_ac >::parse< grammar >( in );
      TAO_PEGTL_TEST_ASSERT( v.size() == 4 );
      verify_sample< one_a >( v[ 0 ], "a", 1, 1, 0 );
      verify_sample< one_c >( v[ 1 ], "c", 1, 2, 1 );
      verify_sample< seq_ac >( v[ 2 ], "ac", 1, 1, 0 );
      verify_sample< sor_ab_ac >( v[ 3 ], "ac", 1, 1, 0 );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void test_failure()
   {
      text_view_input<> in( "ad" );
      const auto v = record< one_a, one_b, one_c, seq_ab >::parse< grammar >( in );
      TAO_PEGTL_TEST_ASSERT( v.empty() );
   }

   void unit_test()
   {
      test_selected_terminals();
      test_first_branch();
      test_second_branch();
      test_failure();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
