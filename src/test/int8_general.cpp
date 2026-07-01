// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_ctrl_enabled< int8::any >( __LINE__, __FILE__, "\x7f" );
      verify_ctrl_disabled< internal::any< internal::peek_int8 > >( __LINE__, __FILE__, "\x7f" );
      verify_ctrl_enabled< int8::many< 2 > >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_int8 > >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_enabled< int8::one< -128, -1, 0, 127 > >( __LINE__, __FILE__, "\x80" );
      verify_ctrl_disabled< internal::one< internal::peek_int8, -128, -1, 0, 127 > >( __LINE__, __FILE__, "\x80" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_int8, -128, -1, 0, 127 >, internal::peek_int8 > >( __LINE__, __FILE__, "\x80" );
      verify_ctrl_enabled< int8::range< -3, 3 > >( __LINE__, __FILE__, "\xfd" );
      verify_ctrl_disabled< internal::range< internal::peek_int8, -3, 3 > >( __LINE__, __FILE__, "\xfd" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_int8, -3, 3 >, internal::peek_int8 > >( __LINE__, __FILE__, "\xfd" );
      verify_ctrl_enabled< int8::not_range< -3, 3 > >( __LINE__, __FILE__, "\xfc" );
      verify_ctrl_disabled< internal::not_range< internal::peek_int8, -3, 3 > >( __LINE__, __FILE__, "\xfc" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_int8, -3, 3 >, internal::peek_int8 > >( __LINE__, __FILE__, "\xfc" );
      verify_ctrl_enabled< int8::ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, "\xf8" );
      verify_ctrl_disabled< internal::ranges< internal::peek_int8, -8, -4, 4, 8 > >( __LINE__, __FILE__, "\xf8" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_int8, -8, -4, 4, 8 >, internal::peek_int8 > >( __LINE__, __FILE__, "\xf8" );
      verify_ctrl_enabled< int8::not_ranges< -8, -4, 4, 8 > >( __LINE__, __FILE__, std::string_view( "\x00", 1 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_int8, -8, -4, 4, 8 > >( __LINE__, __FILE__, std::string_view( "\x00", 1 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_int8, -8, -4, 4, 8 >, internal::peek_int8 > >( __LINE__, __FILE__, std::string_view( "\x00", 1 ) );
      verify_ctrl_enabled< int8::string< -1, 0, 1 > >( __LINE__, __FILE__, std::string_view( "\xff\x00\x01", 3 ) );

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

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
