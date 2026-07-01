// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/uint32.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_ctrl_enabled< uint32_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::any< internal::peek_uint32_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< uint32_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::any< internal::peek_uint32_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< uint32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< uint32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08" );
      verify_ctrl_enabled< uint32_be::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::one< internal::peek_uint32_be, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_uint32_be, 0x01111111, 0x02222222 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_enabled< uint32_le::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::one< internal::peek_uint32_le, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_uint32_le, 0x01111111, 0x02222222 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_enabled< uint32_be::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::not_one< internal::peek_uint32_be, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_uint32_be, 0x01111111, 0x02222222 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_enabled< uint32_le::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::not_one< internal::peek_uint32_le, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_uint32_le, 0x01111111, 0x02222222 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_enabled< uint32_be::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::range< internal::peek_uint32_be, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_uint32_be, 0x01000000, 0x04000000 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_enabled< uint32_le::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::range< internal::peek_uint32_le, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_uint32_le, 0x01000000, 0x04000000 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_enabled< uint32_be::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::not_range< internal::peek_uint32_be, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_uint32_be, 0x01000000, 0x04000000 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_enabled< uint32_le::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::not_range< internal::peek_uint32_le, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_uint32_le, 0x01000000, 0x04000000 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_enabled< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_disabled< internal::ranges< internal::peek_uint32_be, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_uint32_be, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_enabled< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_disabled< internal::ranges< internal::peek_uint32_le, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_uint32_le, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_enabled< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x34\x56\x78" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_uint32_be, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x34\x56\x78" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_uint32_be, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_uint32_be > >( __LINE__, __FILE__, "\x02\x34\x56\x78" );
      verify_ctrl_enabled< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x78\x56\x34\x02" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_uint32_le, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x78\x56\x34\x02" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_uint32_le, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_uint32_le > >( __LINE__, __FILE__, "\x78\x56\x34\x02" );
      verify_ctrl_enabled< uint32_be::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54" );
      verify_ctrl_enabled< uint32_le::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45" );
      verify_ctrl_enabled< uint32_be::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::one< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222 >, internal::peek_mask_uint32_be< 0xffffffff > > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_enabled< uint32_le::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::one< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222 >, internal::peek_mask_uint32_le< 0xffffffff > > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_enabled< uint32_be::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::not_one< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222 >, internal::peek_mask_uint32_be< 0xffffffff > > >( __LINE__, __FILE__, "\x11\x11\x11\x01" );
      verify_ctrl_enabled< uint32_le::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::not_one< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222 >, internal::peek_mask_uint32_le< 0xffffffff > > >( __LINE__, __FILE__, "\x01\x11\x11\x11" );
      verify_ctrl_enabled< uint32_be::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::range< internal::peek_mask_uint32_be< 0xffffffff >, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_mask_uint32_be< 0xffffffff >, 0x01000000, 0x04000000 >, internal::peek_mask_uint32_be< 0xffffffff > > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_enabled< uint32_le::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::range< internal::peek_mask_uint32_le< 0xffffffff >, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_mask_uint32_le< 0xffffffff >, 0x01000000, 0x04000000 >, internal::peek_mask_uint32_le< 0xffffffff > > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_enabled< uint32_be::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::not_range< internal::peek_mask_uint32_be< 0xffffffff >, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_mask_uint32_be< 0xffffffff >, 0x01000000, 0x04000000 >, internal::peek_mask_uint32_be< 0xffffffff > > >( __LINE__, __FILE__, "\x77\x77\x77\x02" );
      verify_ctrl_enabled< uint32_le::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::not_range< internal::peek_mask_uint32_le< 0xffffffff >, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_mask_uint32_le< 0xffffffff >, 0x01000000, 0x04000000 >, internal::peek_mask_uint32_le< 0xffffffff > > >( __LINE__, __FILE__, "\x02\x77\x77\x77" );
      verify_ctrl_enabled< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_disabled< internal::ranges< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_mask_uint32_be< 0xffffffff >, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_mask_uint32_be< 0xffffffff > > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_enabled< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_disabled< internal::ranges< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_mask_uint32_le< 0xffffffff >, 0x01111111, 0x02222222, 0x03333333, 0x04444444 >, internal::peek_mask_uint32_le< 0xffffffff > > >( __LINE__, __FILE__, "\x67\x45\x23\x01" );
      verify_ctrl_enabled< uint32_be::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54" );
      verify_ctrl_enabled< uint32_le::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45" );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "", result_type::local_failure );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x01\x02", result_type::local_failure );
      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x71\x72", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x01\x02", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x71\x72", result_type::local_failure );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x71\x72\x03", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x71\x72\x03", result_type::local_failure );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x55", result_type::success );
      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x71\x72\x03\x55", result_type::success );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x55", result_type::success );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x71\x72\x03\x55", result_type::success );

      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x55\x44", result_type::success, 1 );
      verify_view< uint32_be::any >( __LINE__, __FILE__, "\x71\x72\x03\x55\x44", result_type::success, 1 );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x55\x44", result_type::success, 1 );
      verify_view< uint32_le::any >( __LINE__, __FILE__, "\x71\x72\x03\x55\x44", result_type::success, 1 );

      verify_view< uint32_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< uint32_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< uint32_be::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success, 4 );
      verify_view< uint32_le::many< 0 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success, 4 );
      verify_view< uint32_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint32_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint32_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< uint32_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< uint32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< uint32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07", result_type::local_failure );
      verify_view< uint32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< uint32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08", result_type::success );
      verify_view< uint32_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09", result_type::success, 1 );
      verify_view< uint32_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05\x06\x07\x08\x09", result_type::success, 1 );

      verify_view< uint32_be::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11", result_type::local_failure );

      verify_view< uint32_be::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::local_failure );
      verify_view< uint32_be::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::success );

      verify_view< uint32_le::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::success );
      verify_view< uint32_le::mask_not_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::local_failure );

      verify_view< uint32_be::mask_not_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\xf3\x11\x11\x11", result_type::success );
      verify_view< uint32_be::mask_not_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\xf1\x11\x11\x11", result_type::local_failure );

      verify_view< uint32_le::mask_not_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\xf3", result_type::success );
      verify_view< uint32_le::mask_not_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\xf1", result_type::local_failure );

      verify_view< uint32_be::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::local_failure );
      verify_view< uint32_be::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::success );

      verify_view< uint32_le::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::success );
      verify_view< uint32_le::mask_not_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::local_failure );

      verify_view< uint32_be::mask_not_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x52\x77\x77\x77", result_type::local_failure );
      verify_view< uint32_be::mask_not_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x56\x77\x77\x77", result_type::success );

      verify_view< uint32_le::mask_not_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x52", result_type::local_failure );
      verify_view< uint32_le::mask_not_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x56", result_type::success );

      verify_view< uint32_be::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::success );
      verify_view< uint32_be::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::local_failure );

      verify_view< uint32_le::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::local_failure );
      verify_view< uint32_le::mask_one< 0xffffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::success );

      verify_view< uint32_be::mask_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\xf3\x11\x11\x11", result_type::local_failure );
      verify_view< uint32_be::mask_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\xf1\x11\x11\x11", result_type::success );

      verify_view< uint32_le::mask_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\xf3", result_type::local_failure );
      verify_view< uint32_le::mask_one< 0x0fffffff, 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\xf1", result_type::success );

      verify_view< uint32_be::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::success );
      verify_view< uint32_be::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::local_failure );

      verify_view< uint32_le::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::local_failure );
      verify_view< uint32_le::mask_range< 0xffffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::success );

      verify_view< uint32_be::mask_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x52\x77\x77\x77", result_type::success );
      verify_view< uint32_be::mask_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x56\x77\x77\x77", result_type::local_failure );

      verify_view< uint32_le::mask_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x52", result_type::success );
      verify_view< uint32_le::mask_range< 0x0fffffff, 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x56", result_type::local_failure );

      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::local_failure );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::success );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x89\x12\x34", result_type::local_failure );

      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::success );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::local_failure );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::success );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x34\x12\x89\x67", result_type::local_failure );

      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::local_failure );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::success );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x89\x01\x02", result_type::success );

      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::success );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::local_failure );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::success );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x01\x89\x67", result_type::success );

      verify_view< uint32_be::mask_ranges< 0xff0fffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x50\x02\x02", result_type::success );
      verify_view< uint32_be::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x50\x02\x02", result_type::local_failure );

      verify_view< uint32_le::mask_ranges< 0xff0fffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x02\x50\x02", result_type::success );
      verify_view< uint32_le::mask_ranges< 0xffffffff, 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x02\x50\x02", result_type::local_failure );

      verify_view< uint32_be::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54", result_type::success );
      verify_view< uint32_be::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45", result_type::local_failure );

      verify_view< uint32_le::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54", result_type::local_failure );
      verify_view< uint32_le::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45", result_type::success );

      verify_view< uint32_be::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x81\x23\x32\x10\x45\x67\x76\x54", result_type::local_failure );
      verify_view< uint32_be::mask_string< 0x4fffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x81\x23\x32\x10\x45\x67\x76\x54", result_type::success );

      verify_view< uint32_le::mask_string< 0xffffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x81\x54\x76\x67\x45", result_type::local_failure );
      verify_view< uint32_le::mask_string< 0x4fffffff, 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x81\x54\x76\x67\x45", result_type::success );

      verify_view< uint32_be::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::local_failure );
      verify_view< uint32_be::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::success );

      verify_view< uint32_le::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::success );
      verify_view< uint32_le::not_one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::local_failure );

      verify_view< uint32_be::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::local_failure );
      verify_view< uint32_be::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::success );

      verify_view< uint32_le::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::success );
      verify_view< uint32_le::not_range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::local_failure );

      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::local_failure );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::success );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::local_failure );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x89\x12\x34", result_type::success );

      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::local_failure );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::success );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::local_failure );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x34\x12\x89\x67", result_type::success );

      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::local_failure );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::success );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::local_failure );
      verify_view< uint32_be::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x89\x01\x02", result_type::local_failure );

      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::local_failure );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::success );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::local_failure );
      verify_view< uint32_le::not_ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x01\x89\x67", result_type::local_failure );

      verify_view< uint32_be::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::success );
      verify_view< uint32_be::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::local_failure );

      verify_view< uint32_le::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x01\x11\x11\x11", result_type::local_failure );
      verify_view< uint32_le::one< 0x01111111, 0x02222222 > >( __LINE__, __FILE__, "\x11\x11\x11\x01", result_type::success );

      verify_view< uint32_be::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::success );
      verify_view< uint32_be::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::local_failure );

      verify_view< uint32_le::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x02\x77\x77\x77", result_type::local_failure );
      verify_view< uint32_le::range< 0x01000000, 0x04000000 > >( __LINE__, __FILE__, "\x77\x77\x77\x02", result_type::success );

      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::local_failure );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::success );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x89\x12\x34", result_type::local_failure );

      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::success );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::local_failure );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::success );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444 > >( __LINE__, __FILE__, "\x34\x12\x89\x67", result_type::local_failure );

      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x34\x56\x78", result_type::local_failure );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x03\x45\x67\x89", result_type::success );
      verify_view< uint32_be::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x89\x01\x02", result_type::success );

      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x67\x45\x23\x01", result_type::success );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x78\x56\x34\x02", result_type::local_failure );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x89\x67\x45\x03", result_type::success );
      verify_view< uint32_le::ranges< 0x01111111, 0x02222222, 0x03333333, 0x04444444, 0x67890102 > >( __LINE__, __FILE__, "\x02\x01\x89\x67", result_type::success );

      verify_view< uint32_be::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54", result_type::success );
      verify_view< uint32_be::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45", result_type::local_failure );

      verify_view< uint32_le::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x01\x23\x32\x10\x45\x67\x76\x54", result_type::local_failure );
      verify_view< uint32_le::string< 0x01233210, 0x45677654 > >( __LINE__, __FILE__, "\x10\x32\x23\x01\x54\x76\x67\x45", result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
