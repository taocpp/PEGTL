// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/debug/visit.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct A : plus< alpha, opt< A > > {};
   struct B : star< sor< space, digit, A > > {};
   struct grammar : seq< A, B > {};
   // clang-format on

   void unit_test()
   {
      static_assert( contains_v< grammar, digit > );
      static_assert( !contains_v< grammar, xdigit > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
