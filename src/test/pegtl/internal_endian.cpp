// Copyright (c) 2018-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/endian.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int8_t( 0x7a ) ) == std::int8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint8_t( 0x7a ) ) == std::uint8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int8_t( 0x7a ) ) == std::int8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint8_t( 0x7a ) ) == std::uint8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int8_t( 0x7a ) ) == std::int8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint8_t( 0x7a ) ) == std::uint8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int8_t( 0x7a ) ) == std::int8_t( 0x7a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint8_t( 0x7a ) ) == std::uint8_t( 0x7a ) );
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x397a ) );

      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x2b1f397a ) );

      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x675544332b1f397a ) );
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x7a39 ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int16_t( 0x7a39 ) ) == std::int16_t( 0x397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint16_t( 0x7a39 ) ) == std::uint16_t( 0x397a ) );

      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x7a391f2b ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int32_t( 0x7a391f2b ) ) == std::int32_t( 0x2b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint32_t( 0x7a391f2b ) ) == std::uint32_t( 0x2b1f397a ) );

      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::identity_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::byteswap_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::little_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x7a391f2b33445567 ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::int64_t( 0x7a391f2b33445567 ) ) == std::int64_t( 0x675544332b1f397a ) );
      TAO_PEGTL_TEST_ASSERT( internal::big_endian::get( std::uint64_t( 0x7a391f2b33445567 ) ) == std::uint64_t( 0x675544332b1f397a ) );
#else
      static_assert( false );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
