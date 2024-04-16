// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/debug/internal/set_stack_guard.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::set< int > s = { -5, 0, 5 };
      TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 5 } ) );
      {
         const internal::set_stack_guard g( s, 3 );
         TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 3, 5 } ) );
      }
      TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 5 } ) );
      {
         const internal::set_stack_guard g( s, 5 );
         TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 5 } ) );
      }
      TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 5 } ) );
      {
         const internal::set_stack_guard g( s, 3 );
         TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 3, 5 } ) );
         {
            const internal::set_stack_guard h( s, -3 );
            TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, -3, 0, 3, 5 } ) );
            {
               const internal::set_stack_guard i( s, 0 );
               TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, -3, 0, 3, 5 } ) );
            }
            TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, -3, 0, 3, 5 } ) );
         }
         TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 3, 5 } ) );
      }
      TAO_PEGTL_TEST_ASSERT( s == std::set< int >( { -5, 0, 5 } ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
