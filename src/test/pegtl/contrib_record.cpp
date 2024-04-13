// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/record.hpp>

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

   void test1()
   {
      text_view_input<> in( "ac" );
      const auto v = record2< one_a, one_b, one_c >::parse< grammar >( in );
      std::cerr << v;
      TAO_PEGTL_TEST_ASSERT( v.size() == 2 );
   }

   void test2()
   {
      text_view_input<> in( "ac" );
      const auto v = record2< one_a, one_b, one_c, seq_ab >::parse< grammar >( in );
      std::cerr << v;
      TAO_PEGTL_TEST_ASSERT( v.size() == 2 );
   }

   void unit_test()
   {
      test1();
      test2();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
