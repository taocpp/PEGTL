// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int16.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
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

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
