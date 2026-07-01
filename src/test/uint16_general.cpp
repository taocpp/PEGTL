// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/binary/uint16.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_ctrl_enabled< uint16_be::any >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_disabled< internal::any< internal::peek_uint16_be > >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_enabled< uint16_le::any >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_disabled< internal::any< internal::peek_uint16_le > >( __LINE__, __FILE__, "\x01\x02" );
      verify_ctrl_enabled< uint16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< uint16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_disabled< internal::many< 2, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x01\x02\x03\x04" );
      verify_ctrl_enabled< uint16_be::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::one< internal::peek_uint16_be, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_uint16_be, 0x0111, 0x0222 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_enabled< uint16_le::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::one< internal::peek_uint16_le, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_uint16_le, 0x0111, 0x0222 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_enabled< uint16_be::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x03\x33" );
      verify_ctrl_disabled< internal::not_one< internal::peek_uint16_be, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x03\x33" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_uint16_be, 0x0111, 0x0222 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x03\x33" );
      verify_ctrl_enabled< uint16_le::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x33\x03" );
      verify_ctrl_disabled< internal::not_one< internal::peek_uint16_le, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x33\x03" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_uint16_le, 0x0111, 0x0222 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x33\x03" );
      verify_ctrl_enabled< uint16_be::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::range< internal::peek_uint16_be, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_uint16_be, 0x0100, 0x0400 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_enabled< uint16_le::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::range< internal::peek_uint16_le, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_uint16_le, 0x0100, 0x0400 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_enabled< uint16_be::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::not_range< internal::peek_uint16_be, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_uint16_be, 0x0100, 0x0400 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_enabled< uint16_le::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::not_range< internal::peek_uint16_le, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_uint16_le, 0x0100, 0x0400 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_enabled< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_disabled< internal::ranges< internal::peek_uint16_be, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_uint16_be, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_enabled< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_disabled< internal::ranges< internal::peek_uint16_le, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_uint16_le, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_enabled< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x67\x89" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_uint16_be, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x67\x89" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_uint16_be, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_uint16_be > >( __LINE__, __FILE__, "\x67\x89" );
      verify_ctrl_enabled< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x89\x67" );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_uint16_le, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x89\x67" );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_uint16_le, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_uint16_le > >( __LINE__, __FILE__, "\x89\x67" );
      verify_ctrl_enabled< uint16_be::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_enabled< uint16_le::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45" );
      verify_ctrl_enabled< uint16_be::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::one< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222 >, internal::peek_mask_uint16_be< 0xffff > > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_enabled< uint16_le::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::one< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222 >, internal::peek_mask_uint16_le< 0xffff > > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_enabled< uint16_be::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::not_one< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222 >, internal::peek_mask_uint16_be< 0xffff > > >( __LINE__, __FILE__, "\x11\x01" );
      verify_ctrl_enabled< uint16_le::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::not_one< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222 >, internal::peek_mask_uint16_le< 0xffff > > >( __LINE__, __FILE__, "\x01\x11" );
      verify_ctrl_enabled< uint16_be::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::range< internal::peek_mask_uint16_be< 0xffff >, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_mask_uint16_be< 0xffff >, 0x0100, 0x0400 >, internal::peek_mask_uint16_be< 0xffff > > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_enabled< uint16_le::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::range< internal::peek_mask_uint16_le< 0xffff >, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_mask_uint16_le< 0xffff >, 0x0100, 0x0400 >, internal::peek_mask_uint16_le< 0xffff > > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_enabled< uint16_be::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::not_range< internal::peek_mask_uint16_be< 0xffff >, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_mask_uint16_be< 0xffff >, 0x0100, 0x0400 >, internal::peek_mask_uint16_be< 0xffff > > >( __LINE__, __FILE__, "\x77\x02" );
      verify_ctrl_enabled< uint16_le::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::not_range< internal::peek_mask_uint16_le< 0xffff >, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_mask_uint16_le< 0xffff >, 0x0100, 0x0400 >, internal::peek_mask_uint16_le< 0xffff > > >( __LINE__, __FILE__, "\x02\x77" );
      verify_ctrl_enabled< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_disabled< internal::ranges< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_mask_uint16_be< 0xffff >, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_mask_uint16_be< 0xffff > > >( __LINE__, __FILE__, "\x01\x23" );
      verify_ctrl_enabled< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_disabled< internal::ranges< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_mask_uint16_le< 0xffff >, 0x0111, 0x0222, 0x0333, 0x0444 >, internal::peek_mask_uint16_le< 0xffff > > >( __LINE__, __FILE__, "\x23\x01" );
      verify_ctrl_enabled< uint16_be::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67" );
      verify_ctrl_enabled< uint16_le::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45" );

      verify_view< uint16_be::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "", result_type::local_failure );

      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );

      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x71\x72", result_type::success );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x71\x72", result_type::success );

      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x01\x02\x03", result_type::success, 1 );
      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x71\x72\x03", result_type::success, 1 );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x01\x02\x03", result_type::success, 1 );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x71\x72\x03", result_type::success, 1 );

      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x01\x02\x03\x55", result_type::success, 2 );
      verify_view< uint16_be::any >( __LINE__, __FILE__, "\x71\x72\x03\x55", result_type::success, 2 );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x01\x02\x03\x55", result_type::success, 2 );
      verify_view< uint16_le::any >( __LINE__, __FILE__, "\x71\x72\x03\x55", result_type::success, 2 );

      verify_view< uint16_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< uint16_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_view< uint16_be::many< 0 > >( __LINE__, __FILE__, "\x01\x02", result_type::success, 2 );
      verify_view< uint16_le::many< 0 > >( __LINE__, __FILE__, "\x01\x02", result_type::success, 2 );
      verify_view< uint16_be::many< 1 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< uint16_le::many< 1 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_view< uint16_be::many< 1 > >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< uint16_le::many< 1 > >( __LINE__, __FILE__, "\x01\x02", result_type::success );
      verify_view< uint16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03", result_type::local_failure );
      verify_view< uint16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< uint16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04", result_type::success );
      verify_view< uint16_be::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05", result_type::success, 1 );
      verify_view< uint16_le::many< 2 > >( __LINE__, __FILE__, "\x01\x02\x03\x04\x05", result_type::success, 1 );

      verify_view< uint16_be::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01", result_type::local_failure );

      verify_view< uint16_be::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::local_failure );
      verify_view< uint16_be::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::success );

      verify_view< uint16_le::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::success );
      verify_view< uint16_le::mask_not_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::local_failure );

      verify_view< uint16_be::mask_not_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\xf3\x11", result_type::success );
      verify_view< uint16_be::mask_not_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\xf1\x11", result_type::local_failure );

      verify_view< uint16_le::mask_not_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\xf3", result_type::success );
      verify_view< uint16_le::mask_not_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\xf1", result_type::local_failure );

      verify_view< uint16_be::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::local_failure );
      verify_view< uint16_be::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::success );

      verify_view< uint16_le::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::success );
      verify_view< uint16_le::mask_not_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::local_failure );

      verify_view< uint16_be::mask_not_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x52\x77", result_type::local_failure );
      verify_view< uint16_be::mask_not_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x56\x77", result_type::success );

      verify_view< uint16_le::mask_not_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x52", result_type::local_failure );
      verify_view< uint16_le::mask_not_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x56", result_type::success );

      verify_view< uint16_be::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::success );
      verify_view< uint16_be::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::local_failure );

      verify_view< uint16_le::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::local_failure );
      verify_view< uint16_le::mask_one< 0xffff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::success );

      verify_view< uint16_be::mask_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\xf3\x11", result_type::local_failure );
      verify_view< uint16_be::mask_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\xf1\x11", result_type::success );

      verify_view< uint16_le::mask_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\xf3", result_type::local_failure );
      verify_view< uint16_le::mask_one< 0x0fff, 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\xf1", result_type::success );

      verify_view< uint16_be::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::success );
      verify_view< uint16_be::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::local_failure );

      verify_view< uint16_le::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::local_failure );
      verify_view< uint16_le::mask_range< 0xffff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::success );

      verify_view< uint16_be::mask_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x52\x77", result_type::success );
      verify_view< uint16_be::mask_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x56\x77", result_type::local_failure );

      verify_view< uint16_le::mask_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x52", result_type::success );
      verify_view< uint16_le::mask_range< 0x0fff, 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x56", result_type::local_failure );

      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23", result_type::success );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x02\x34", result_type::local_failure );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x03\x45", result_type::success );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x67\x89", result_type::local_failure );

      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01", result_type::success );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x34\x02", result_type::local_failure );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x45\x03", result_type::success );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x89\x67", result_type::local_failure );

      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x01\x23", result_type::success );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x02\x34", result_type::local_failure );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x03\x45", result_type::success );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x67\x89", result_type::success );

      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x23\x01", result_type::success );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x34\x02", result_type::local_failure );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x45\x03", result_type::success );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x89\x67", result_type::success );

      verify_view< uint16_be::mask_ranges< 0xff0f, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x02\x50", result_type::success );
      verify_view< uint16_be::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x02\x50", result_type::local_failure );

      verify_view< uint16_le::mask_ranges< 0xff0f, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x50\x02", result_type::success );
      verify_view< uint16_le::mask_ranges< 0xffff, 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x50\x02", result_type::local_failure );

      verify_view< uint16_be::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint16_be::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45", result_type::local_failure );

      verify_view< uint16_le::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::local_failure );
      verify_view< uint16_le::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45", result_type::success );

      verify_view< uint16_be::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x81\x23\x45\x67", result_type::local_failure );
      verify_view< uint16_be::mask_string< 0x4fff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x81\x23\x45\x67", result_type::success );

      verify_view< uint16_le::mask_string< 0xffff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x81\x67\x45", result_type::local_failure );
      verify_view< uint16_le::mask_string< 0x4fff, 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x81\x67\x45", result_type::success );

      verify_view< uint16_be::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::local_failure );
      verify_view< uint16_be::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::success );

      verify_view< uint16_le::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::success );
      verify_view< uint16_le::not_one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::local_failure );

      verify_view< uint16_be::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::local_failure );
      verify_view< uint16_be::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::success );

      verify_view< uint16_le::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::success );
      verify_view< uint16_le::not_range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::local_failure );

      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23", result_type::local_failure );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x02\x34", result_type::success );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x03\x45", result_type::local_failure );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x67\x89", result_type::success );

      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01", result_type::local_failure );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x34\x02", result_type::success );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x45\x03", result_type::local_failure );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x89\x67", result_type::success );

      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x01\x23", result_type::local_failure );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x02\x34", result_type::success );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x03\x45", result_type::local_failure );
      verify_view< uint16_be::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x67\x89", result_type::local_failure );

      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x23\x01", result_type::local_failure );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x34\x02", result_type::success );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x45\x03", result_type::local_failure );
      verify_view< uint16_le::not_ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x89\x67", result_type::local_failure );

      verify_view< uint16_be::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::success );
      verify_view< uint16_be::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::local_failure );

      verify_view< uint16_le::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x01\x11", result_type::local_failure );
      verify_view< uint16_le::one< 0x0111, 0x0222 > >( __LINE__, __FILE__, "\x11\x01", result_type::success );

      verify_view< uint16_be::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::success );
      verify_view< uint16_be::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::local_failure );

      verify_view< uint16_le::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x02\x77", result_type::local_failure );
      verify_view< uint16_le::range< 0x0100, 0x0400 > >( __LINE__, __FILE__, "\x77\x02", result_type::success );

      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x01\x23", result_type::success );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x02\x34", result_type::local_failure );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x03\x45", result_type::success );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x67\x89", result_type::local_failure );

      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x23\x01", result_type::success );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x34\x02", result_type::local_failure );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x45\x03", result_type::success );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444 > >( __LINE__, __FILE__, "\x89\x67", result_type::local_failure );

      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x01\x23", result_type::success );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x02\x34", result_type::local_failure );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x03\x45", result_type::success );
      verify_view< uint16_be::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x67\x89", result_type::success );

      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x23\x01", result_type::success );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x34\x02", result_type::local_failure );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x45\x03", result_type::success );
      verify_view< uint16_le::ranges< 0x0111, 0x0222, 0x0333, 0x0444, 0x6789 > >( __LINE__, __FILE__, "\x89\x67", result_type::success );

      verify_view< uint16_be::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::success );
      verify_view< uint16_be::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45", result_type::local_failure );

      verify_view< uint16_le::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x01\x23\x45\x67", result_type::local_failure );
      verify_view< uint16_le::string< 0x0123, 0x4567 > >( __LINE__, __FILE__, "\x23\x01\x67\x45", result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
