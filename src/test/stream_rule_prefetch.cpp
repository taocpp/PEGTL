// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include "endless_stream.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< prefetch< 0 >, internal::success >();
      verify_meta< prefetch< 1 >, prefetch< 1 > >();
      verify_meta< prefetch< 1000 >, prefetch< 1000 > >();

      verify_analyze< prefetch< 0 > >( __LINE__, __FILE__, false, false );
      verify_analyze< prefetch< 1 > >( __LINE__, __FILE__, false, false );
      verify_analyze< prefetch< 9 > >( __LINE__, __FILE__, false, false );

      verify_rule< prefetch< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< prefetch< 0 > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< prefetch< 0 > >( __LINE__, __FILE__, "  ", result_type::success, 2 );
      verify_rule< prefetch< 1 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< prefetch< 1 > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< prefetch< 1 > >( __LINE__, __FILE__, "  ", result_type::success, 2 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "1", result_type::success, 1 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "12", result_type::success, 2 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "123", result_type::success, 3 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "1234", result_type::success, 4 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "12345", result_type::success, 5 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "123456", result_type::success, 6 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "1234567", result_type::success, 7 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "12345678", result_type::success, 8 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "123456789", result_type::success, 9 );
      verify_rule< prefetch< 9 > >( __LINE__, __FILE__, "123456789123456789", result_type::success, 18 );

      alloc_endless_input< void > dei( 100, 20, "abcdefghijklmnopqrstuvwxyz" );

      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 0 );
      TAO_PEGTL_TEST_ASSERT( parse< prefetch< 10 > >( dei ) );
      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 20 );
      TAO_PEGTL_TEST_ASSERT( dei.current() == dei.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( parse< prefetch< 30 > >( dei ) );
      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 40 );
      TAO_PEGTL_TEST_ASSERT( dei.current() == dei.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( parse< prefetch< 30 > >( dei ) );
      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 40 );
      TAO_PEGTL_TEST_ASSERT( dei.current() == dei.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( parse< consume< 30 > >( dei ) );
      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 10 );
      TAO_PEGTL_TEST_ASSERT( dei.current() == dei.buffer_start() + 30 );
      TAO_PEGTL_TEST_ASSERT( parse< discard >( dei ) );
      TAO_PEGTL_TEST_ASSERT( dei.buffer_used_size() == 10 );
      TAO_PEGTL_TEST_ASSERT( dei.current() == dei.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( parse< one< 'e' > >( dei ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
