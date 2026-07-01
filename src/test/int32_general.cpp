// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int32.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_ctrl_enabled< int32_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::any< internal::peek_int32_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< int32_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::any< internal::peek_int32_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< int32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_int32_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< int32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_int32_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< int32_be::one< -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::one< internal::peek_int32_be, -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_int32_be, -2147483647 - 1, -1, 0, 2147483647 >, internal::peek_int32_be > >( __LINE__, __FILE__, std::string_view( "\x80\x00\x00\x00", 4 ) );
      verify_ctrl_enabled< int32_le::one< -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x80", 4 ) );
      verify_ctrl_disabled< internal::one< internal::peek_int32_le, -2147483647 - 1, -1, 0, 2147483647 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x80", 4 ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_int32_le, -2147483647 - 1, -1, 0, 2147483647 >, internal::peek_int32_le > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x80", 4 ) );
      verify_ctrl_enabled< int32_be::range< -300000, -200000 > >( __LINE__, __FILE__, "\xff\xfb\x6c\x20" );
      verify_ctrl_disabled< internal::range< internal::peek_int32_be, -300000, -200000 > >( __LINE__, __FILE__, "\xff\xfb\x6c\x20" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_int32_be, -300000, -200000 >, internal::peek_int32_be > >( __LINE__, __FILE__, "\xff\xfb\x6c\x20" );
      verify_ctrl_enabled< int32_le::range< -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff" );
      verify_ctrl_disabled< internal::range< internal::peek_int32_le, -300000, -200000 > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_int32_le, -300000, -200000 >, internal::peek_int32_le > >( __LINE__, __FILE__, "\x20\x6c\xfb\xff" );
      verify_ctrl_enabled< int32_be::ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x03\x0d\x40", 4 ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_int32_be, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x03\x0d\x40", 4 ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_int32_be, -300000, -200000, 200000, 300000 >, internal::peek_int32_be > >( __LINE__, __FILE__, std::string_view( "\x00\x03\x0d\x40", 4 ) );
      verify_ctrl_enabled< int32_be::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_int32_be, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_int32_be, -300000, -200000, 200000, 300000 >, internal::peek_int32_be > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_enabled< int32_le::not_ranges< -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_int32_le, -300000, -200000, 200000, 300000 > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_int32_le, -300000, -200000, 200000, 300000 >, internal::peek_int32_le > >( __LINE__, __FILE__, std::string_view( "\x00\x00\x00\x00", 4 ) );
      verify_ctrl_enabled< int32_be::string< -2, 0x12345678 > >( __LINE__, __FILE__, "\xff\xff\xff\xfe\x12\x34\x56\x78" );
      verify_ctrl_enabled< int32_le::string< -2, 0x12345678 > >( __LINE__, __FILE__, "\xfe\xff\xff\xff\x78\x56\x34\x12" );

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

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
