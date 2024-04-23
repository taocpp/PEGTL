// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/uint16.hpp>
#include <tao/pegtl/uint32.hpp>
#include <tao/pegtl/uint64.hpp>
#include <tao/pegtl/uint8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test16()
   {
      const char data[] = { 2, 2, 1, 2, 2 };
      base_input<> in( data, sizeof( data ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint16::one< 0x0202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint16::one< 0x0202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void test32()
   {
      const char data[] = { 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2 };
      base_input<> in( data, sizeof( data ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint32::one< 0x02020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint32::one< 0x02020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint32::one< 0x02020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint32::one< 0x02020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint32::one< 0x02020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void test64()
   {
      const char data[] = { 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
      base_input<> in( data, sizeof( data ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint8::one< 0x01 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( parse< uint64::one< 0x0202020202020202 > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void unit_test()
   {
      test16();
      test32();
      test64();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
