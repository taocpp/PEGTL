// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include "endless_stream.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< is_stream, is_stream >();

      verify_analyze< is_stream >( __LINE__, __FILE__, false, false );

      alloc_endless_input< void > dei( 100, 20, "abcdefghijklmnopqrstuvwxyz" );
      TAO_PEGTL_TEST_ASSERT( parse< is_stream >( dei ) );
      array_endless_input< void > sei( "abcdefghijklmnopqrstuvwxyz" );
      TAO_PEGTL_TEST_ASSERT( parse< is_stream >( sei ) );
      base_input< void > bi( "abcdefghijklmnopqrstuvwxyz" );
      TAO_PEGTL_TEST_ASSERT( !parse< is_stream >( bi ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
