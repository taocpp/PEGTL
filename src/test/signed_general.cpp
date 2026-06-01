// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int16.hpp>
#include <tao/pegtl/binary/int32.hpp>
#include <tao/pegtl/binary/int64.hpp>
#include <tao/pegtl/binary/int8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test_int8()
   {
      verify_view< int8::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_view< int8::any >( __LINE__, __FILE__, "\x7f", result_type::success );
      verify_view< int8::any >( __LINE__, __FILE__, "\x80", result_type::success );
      verify_view< int8::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int8::many< 0 > >( __LINE__, __FILE__, "\x7f", result_type::success, 1 );
      verify_view< int8::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_view< int8::many< 1 > >( __LINE__, __FILE__, "\x80", result_type::success );
      verify_view< int8::many< 2 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< int8::many< 2 > >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< int8::one< -128, -1, 0, 127 > >( __LINE__, __FILE__, "\x80", result_type::success );
      verify_view< int8::one< -128, -1, 0, 127 > >( __LINE__, __FILE__, "\xff", result_type::success );
      verify_view< int8::one< -128, -1, 0, 127 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< int8::range< -3, 3 > >( __LINE__, __FILE__, "\xfd", result_type::success );
      verify_view< int8::range< -3, 3 > >( __LINE__, __FILE__, "\x03", result_type::success );
      verify_view< int8::range< -3, 3 > >( __LINE__, __FILE__, "\xfc", result_type::local_failure );
      verify_view< int8::not_range< -3, 3 > >( __LINE__, __FILE__, "\xfc", result_type::success );
      verify_view< int8::ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, "\xf8", result_type::success );
      verify_view< int8::ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, "\x06", result_type::success );
      verify_view< int8::ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, std::string( "\x00", 1 ), result_type::local_failure );
      verify_view< int8::not_ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, "\xf8", result_type::local_failure );
      verify_view< int8::not_ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, "\x06", result_type::local_failure );
      verify_view< int8::not_ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, std::string( "\x00", 1 ), result_type::success );
      verify_view< int8::not_ranges< -8, -4, 0, 4, 8 > >( __LINE__, __FILE__, std::string( "\x00", 1 ), result_type::local_failure );
      verify_view< int8::string< -1, 0, 1 > >( __LINE__, __FILE__, std::string( "\xff\x00\x01", 3 ), result_type::success );
      verify_view< int8::string< -1, 0, 1 > >( __LINE__, __FILE__, std::string( "\xff\x00\x02", 3 ), result_type::local_failure );
   }

   void test_int16()
   {
      verify_view< int16_be::any >( __LINE__, __FILE__, "\x80", result_type::local_failure );
      verify_view< int16_le::any >( __LINE__, __FILE__, "\x80", result_type::local_failure );
      verify_view< int16_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int16_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int16_be::many< 0 > >( __LINE__, __FILE__, "\x01\x02", result_type::success, 2 );
      verify_view< int16_le::many< 0 > >( __LINE__, __FILE__, "\x01\x02", result_type::success, 2 );
      verify_view< int16_be::many< 1 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< int16_le::many< 1 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< int16_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< int16_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< int16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< int16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< int16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< int16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< int16_be::one< -32768, -1, 0, 32767 > >( __LINE__, __FILE__, std::string( "\x80\x00", 2 ), result_type::success );
      verify_view< int16_le::one< -32768, -1, 0, 32767 > >( __LINE__, __FILE__, std::string( "\x00\x80", 2 ), result_type::success );
      verify_view< int16_be::one< -32768, -1, 0, 32767 > >( __LINE__, __FILE__, std::string( "\x00\x80", 2 ), result_type::local_failure );
      verify_view< int16_be::range< -300, -200 > >( __LINE__, __FILE__, "\xfe\xd4", result_type::success );
      verify_view< int16_le::range< -300, -200 > >( __LINE__, __FILE__, "\xd4\xfe", result_type::success );
      verify_view< int16_be::range< -300, -200 > >( __LINE__, __FILE__, "\xff\x39", result_type::local_failure );
      verify_view< int16_be::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, "\xfe\xd4", result_type::local_failure );
      verify_view< int16_le::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, "\xd4\xfe", result_type::local_failure );
      verify_view< int16_be::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, std::string( "\x00\xc8", 2 ), result_type::local_failure );
      verify_view< int16_le::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, std::string( "\xc8\x00", 2 ), result_type::local_failure );
      verify_view< int16_be::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, "\xff\x39", result_type::success );
      verify_view< int16_le::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, "\x39\xff", result_type::success );
      verify_view< int16_be::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, std::string( "\x00\x00", 2 ), result_type::success );
      verify_view< int16_le::not_ranges< -300, -200, 200, 300 > >( __LINE__, __FILE__, std::string( "\x00\x00", 2 ), result_type::success );
      verify_view< int16_be::string< -2, 0x1234 > >( __LINE__, __FILE__, "\xff\xfe\x12\x34", result_type::success );
      verify_view< int16_le::string< -2, 0x1234 > >( __LINE__, __FILE__, "\xfe\xff\x34\x12", result_type::success );
   }

   void test_int32()
   {
      verify_view< int32_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int32_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int32_be::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success, 4 );
      verify_view< int32_le::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success, 4 );
      verify_view< int32_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< int32_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< int32_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< int32_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< int32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< int32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< int32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< int32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< int32_be::one< -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string( "\x80\x00\x00\x00", 4 ), result_type::success );
      verify_view< int32_le::one< -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x80", 4 ), result_type::success );
      verify_view< int32_be::range< -300000, -200000 > >( __LINE__, __FILE__, "\xff\xfb\x6c\x20", result_type::success );
      verify_view< int32_le::range< -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff", result_type::success );
      verify_view< int32_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x03\x0d\x40", 4 ), result_type::success );
      verify_view< int32_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00", 4 ), result_type::local_failure );
      verify_view< int32_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, "\xff\xfb\x6c\x20", result_type::local_failure );
      verify_view< int32_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff", result_type::local_failure );
      verify_view< int32_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x03\x0d\x40", 4 ), result_type::local_failure );
      verify_view< int32_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x40\x0d\x03\x00", 4 ), result_type::local_failure );
      verify_view< int32_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00", 4 ), result_type::success );
      verify_view< int32_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00", 4 ), result_type::success );
      verify_view< int32_be::string< -2, 0x12345678 > >( __LINE__, __FILE__, "\xff\xff\xff\xfe\x12\x34\x56\x78", result_type::success );
      verify_view< int32_le::string< -2, 0x12345678 > >( __LINE__, __FILE__, "\xfe\xff\xff\xff\x78\x56\x34\x12", result_type::success );
   }

   void test_int64()
   {
      verify_view< int64_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int64_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< int64_be::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success, 8 );
      verify_view< int64_le::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success, 8 );
      verify_view< int64_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< int64_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< int64_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< int64_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< int64_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f", result_type::local_failure );
      verify_view< int64_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f", result_type::local_failure );
      verify_view< int64_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10", result_type::success );
      verify_view< int64_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10", result_type::success );
      verify_view< int64_be::one< -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string( "\x80\x00\x00\x00\x00\x00\x00\x00", 8 ), result_type::success );
      verify_view< int64_le::one< -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x00\x00\x80", 8 ), result_type::success );
      verify_view< int64_be::range< -300000, -200000 > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xfb\x6c\x20", result_type::success );
      verify_view< int64_le::range< -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff\xff\xff\xff\xff", result_type::success );
      verify_view< int64_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x03\x0d\x40", 8 ), result_type::success );
      verify_view< int64_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ), result_type::local_failure );
      verify_view< int64_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xfb\x6c\x20", result_type::local_failure );
      verify_view< int64_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff\xff\xff\xff\xff", result_type::local_failure );
      verify_view< int64_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x03\x0d\x40", 8 ), result_type::local_failure );
      verify_view< int64_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x40\x0d\x03\x00\x00\x00\x00\x00", 8 ), result_type::local_failure );
      verify_view< int64_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ), result_type::success );
      verify_view< int64_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ), result_type::success );
      verify_view< int64_be::string< -2, 0x123456789abcdefLL > >( __LINE__, __FILE__, std::string( "\xff\xff\xff\xff\xff\xff\xff\xfe\x01\x23\x45\x67\x89\xab\xcd\xef", 16 ), result_type::success );
      verify_view< int64_le::string< -2, 0x123456789abcdefLL > >( __LINE__, __FILE__, std::string( "\xfe\xff\xff\xff\xff\xff\xff\xff\xef\xcd\xab\x89\x67\x45\x23\x01", 16 ), result_type::success );
   }

   void unit_test()
   {
      test_int8();
      test_int16();
      test_int32();
      test_int64();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
