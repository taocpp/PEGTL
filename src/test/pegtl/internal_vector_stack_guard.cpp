// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/debug/internal.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::vector< int > v = { 42, 0, 500 };
      TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500 } ) );
      {
         const internal::vector_stack_guard g( v, 1 );
         TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500, 1 } ) );
      }
      TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500 } ) );
      {
         const internal::vector_stack_guard g( v, -5 );
         TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500, -5 } ) );
         {
            const internal::vector_stack_guard h( v, 99 );
            TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500, -5, 99 } ) );
         }
         TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500, -5 } ) );
      }
      TAO_PEGTL_TEST_ASSERT( v == std::vector< int >( { 42, 0, 500 } ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
