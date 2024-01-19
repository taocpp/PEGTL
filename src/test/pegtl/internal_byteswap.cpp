// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/byteswap.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int8_t( 0xd3 ) ) == std::int8_t( 0xd3 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int8_t( 0x27 ) ) == std::int8_t( 0x27 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint8_t( 0x42 ) ) == std::uint8_t( 0x42 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint8_t( 0xc1 ) ) == std::uint8_t( 0xc1 ) );

      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int16_t( 0xfabc ) ) == std::int16_t( 0xbcfa ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int16_t( 0x5421 ) ) == std::int16_t( 0x2154 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint16_t( 0x2105 ) ) == std::uint16_t( 0x0521 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint16_t( 0xabcd ) ) == std::uint16_t( 0xcdab ) );

      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int32_t( 0x12345678 ) ) == std::int32_t( 0x78563412 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint32_t( 0xabcdef01 ) ) == std::uint32_t( 0x01efcdab ) );

      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::int64_t( 0x1234567811223344 ) ) == std::int64_t( 0x4433221178563412 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap( std::uint64_t( 0xabcdef0199887766 ) ) == std::uint64_t( 0x6677889901efcdab ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
