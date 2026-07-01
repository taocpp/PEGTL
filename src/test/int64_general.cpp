// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int64.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_ctrl_enabled< int64_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::any< internal::peek_int64_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< int64_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::any< internal::peek_int64_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< int64_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_int64_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10" );
      verify_ctrl_enabled< int64_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_int64_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10" );
      verify_ctrl_enabled< int64_be::one< -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::one< internal::peek_int64_be, -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_int64_be, -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL >, internal::peek_int64_be > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_enabled< int64_le::one< -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x80", 8 ) );
      verify_ctrl_disabled< internal::one< internal::peek_int64_le, -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x80", 8 ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_int64_le, -9223372036854775807LL - 1, -1, 0, 9223372036854775807LL >, internal::peek_int64_le > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x80", 8 ) );
      verify_ctrl_enabled< int64_be::range< -300000, -200000 > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xfb\x6c\x20" );
      verify_ctrl_disabled< internal::range< internal::peek_int64_be, -300000, -200000 > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xfb\x6c\x20" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_int64_be, -300000, -200000 >, internal::peek_int64_be > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xfb\x6c\x20" );
      verify_ctrl_enabled< int64_le::range< -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff\xff\xff\xff\xff" );
      verify_ctrl_disabled< internal::range< internal::peek_int64_le, -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff\xff\xff\xff\xff" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_int64_le, -300000, -200000 >, internal::peek_int64_le > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff\xff\xff\xff\xff" );
      verify_ctrl_enabled< int64_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x03\x0d\x40", 8 ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_int64_be, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x03\x0d\x40", 8 ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_int64_be, -300000, -200000, 200000, 300000 >, internal::peek_int64_be > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x03\x0d\x40", 8 ) );
      verify_ctrl_enabled< int64_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_int64_be, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_int64_be, -300000, -200000, 200000, 300000 >, internal::peek_int64_be > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_enabled< int64_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_int64_le, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_int64_le, -300000, -200000, 200000, 300000 >, internal::peek_int64_le > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00\x00\x00\x00\x00", 8 ) );
      verify_ctrl_enabled< int64_be::string< -2, 0x123456789abcdefLL > >( __LINE__, __FILE__, "\xff\xff\xff\xff\xff\xff\xff\xfe\x01\x23\x45\x67\x89\xab\xcd\xef" );
      verify_ctrl_enabled< int64_le::string< -2, 0x123456789abcdefLL > >( __LINE__, __FILE__, "\xfe\xff\xff\xff\xff\xff\xff\xff\xef\xcd\xab\x89\x67\x45\x23\x01" );

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

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
