// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/int8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
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

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
