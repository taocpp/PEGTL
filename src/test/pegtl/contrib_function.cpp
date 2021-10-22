// Copyright (c) 2020-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/function.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   bool call1 = false;

   [[nodiscard]] bool func1( memory_input<>& /*unused*/, int /*unused*/, char*& /*unused*/, const double& /*unused*/ )
   {
      call1 = true;
      return true;
   }

   struct rule1 : TAO_PEGTL_NAMESPACE::function< func1 >
   {};

   void unit_test()
   {
      int i = 42;
      char c = 'a';
      double d = 42.0;
      memory_input in( "foo", __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( parse< rule1 >( in, i, &c, d ) );
      TAO_PEGTL_TEST_ASSERT( call1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
