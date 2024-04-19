// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/stream_to_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( internal::stream_to_string().empty() );
      TAO_PEGTL_TEST_ASSERT( internal::stream_to_string( "foo" ) == "foo" );
      TAO_PEGTL_TEST_ASSERT( internal::stream_to_string( "foo", 42, "bar" ) == "foo42bar" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
