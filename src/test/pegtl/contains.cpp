// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <type_traits>

#include <tao/pegtl/contrib/contains.hpp>

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
      bool result = false;

      visit< grammar, contains< xdigit >::visitor >( result );
      TAO_PEGTL_TEST_ASSERT( result == false );

      visit< grammar, contains< digit >::visitor >( result );
      TAO_PEGTL_TEST_ASSERT( result == true );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
