// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <type_traits>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct A : plus< alpha, opt< A > > {};
   struct B : star< sor< space, digit, A > > {};
   struct grammar : seq< A, B > {};
   // clang-format on

   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( contains_v< grammar, digit > == true );
      TAO_PEGTL_TEST_ASSERT( contains_v< grammar, xdigit > == false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
