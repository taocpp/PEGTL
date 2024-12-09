// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/resize_uninitialized.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::string s1;
      TAO_PEGTL_TEST_ASSERT( s1.size() == 0 );
      TAO_PEGTL_TEST_ASSERT( s1.capacity() >= 0 );
      internal::resize_uninitialized( s1, 100 );
      TAO_PEGTL_TEST_ASSERT( s1.size() == 100 );
      TAO_PEGTL_TEST_ASSERT( s1.capacity() >= 100 );
      internal::resize_uninitialized( s1, 42 );
      TAO_PEGTL_TEST_ASSERT( s1.size() == 42 );
      TAO_PEGTL_TEST_ASSERT( s1.capacity() >= 100 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
