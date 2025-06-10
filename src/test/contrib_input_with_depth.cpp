// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string_view>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/contrib/input_with_depth.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using input_t = input_with_depth< text_view_input< scan::lf > >;
      const std::string_view data = "test";
      input_t in( data );
      TAO_PEGTL_TEST_ASSERT( in.current() == data.data() );
      TAO_PEGTL_TEST_ASSERT( in.current_depth() == 0 );
      {
         const auto dg = in.make_depth_guard();
         TAO_PEGTL_TEST_ASSERT( in.current_depth() == 1 );
         in.consume< alpha >( 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == data.data() + 2 );
      }
      TAO_PEGTL_TEST_ASSERT( in.current() == data.data() + 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_depth() == 0 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
