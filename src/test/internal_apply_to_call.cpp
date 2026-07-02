// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/action/internal/apply_to_call.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      int value = 0;

      int member_nothrow( const int a, const int b ) noexcept
      {
         return value = 300 + a + b;
      }

      int member_throws( const int a, const int b )
      {
         return value = 400 + a + b;
      }
   };

   int free_nothrow( test_target& target, const int a, const int b ) noexcept
   {
      return target.value = 100 + a + b;
   }

   int free_throws( test_target& target, const int a, const int b )
   {
      return target.value = 200 + a + b;
   }

   void unit_test()
   {
      test_target target;

      TAO_PEGTL_TEST_ASSERT( internal::apply_to_call< &test_target::value >::call( target, 11 ) == 11 );
      TAO_PEGTL_TEST_ASSERT( target.value == 11 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( internal::apply_to_call< &test_target::value, 22 >::call( target ) == 22 );
      TAO_PEGTL_TEST_ASSERT( target.value == 22 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_nothrow, 2, 1 >::call( target ) == 103 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 103 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_nothrow, 2 >::call( target, 1 ) == 103 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 103 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_nothrow >::call( target, 2, 1 ) == 103 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 103 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_throws, 4, 3 >::call( target ) == 207 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 207 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_throws, 4 >::call( target, 3 ) == 207 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 207 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &free_throws >::call( target, 4, 3 ) == 207 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 207 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_nothrow, 6, 5 >::call( target ) == 311 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 311 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_nothrow, 6 >::call( target, 5 ) == 311 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 311 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_nothrow >::call( target, 6, 5 ) == 311 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 311 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_throws, 8, 7 >::call( target ) == 415 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 415 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_throws, 8 >::call( target, 7 ) == 415 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 415 );

      target.value = 0;

      TAO_PEGTL_TEST_ASSERT( ( internal::apply_to_call< &test_target::member_throws >::call( target, 8, 7 ) == 415 ) );
      TAO_PEGTL_TEST_ASSERT( target.value == 415 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
