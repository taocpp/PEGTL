// Copyright (c) 2015-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_endian.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/unicode/utf16.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace
   {
      [[nodiscard]] std::string u16s( const char16_t u )
      {
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

      [[nodiscard]] std::string u16s_be( const char16_t v )
      {
         const std::uint16_t u = test::h_to_be( static_cast< std::uint16_t >( v ) );
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

      [[nodiscard]] std::string u16s_le( const char16_t v )
      {
         const std::uint16_t u = test::h_to_le( static_cast< std::uint16_t >( v ) );
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

   }  // namespace

   void test_utf16()
   {
      verify_ctrl_enabled< utf16::any >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20ac ) );

      verify_rule< utf16::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, "\xff", result_type::local_failure );

      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 1 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, "  ", result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x00ff ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x0100 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x0fff ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x1000 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd900 ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xde00 ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xfffe ) + " ", result_type::success, 1 );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xffff ) + "  ", result_type::success, 2 );

      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd7ff ) + u16s( 0xdfff ), result_type::success, 2 );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdc00 ) + u16s( 0xdfff ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0x0020 ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xff20 ), result_type::local_failure );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xdf00 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xdfff ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdbff ) + u16s( 0xdc00 ), result_type::success );
      verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdbff ) + u16s( 0xdfff ), result_type::success );

      verify_rule< utf16::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< utf16::many< 0 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::success, 2 );
      verify_rule< utf16::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16::many< 1 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::success );
      verify_rule< utf16::many< 2 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::local_failure );
      verify_rule< utf16::many< 2 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ), result_type::success );
      verify_rule< utf16::many< 2 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0xd801 ) + u16s( 0xdc37 ) + u16s( 0x20 ), result_type::success, 2 );

      verify_ctrl_enabled< utf16::many< 2 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ) );

      verify_rule< utf16::one< 0x20 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::success );
      verify_rule< utf16::one< 0x20ac > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::success );
      verify_rule< utf16::one< 0x10437 > >( __LINE__, __FILE__, u16s( 0xd801 ) + u16s( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16::one< 0x20ac > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16, 0x20ac > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16, 0x20ac >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20ac ) );

      verify_rule< utf16::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::local_failure );
      verify_rule< utf16::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s( 0xd801 ) + u16s( 0xdc37 ), result_type::local_failure );
      verify_rule< utf16::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf16, 0x20, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf16, 0x20, 0x10437 >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20ac ) );

      verify_rule< utf16::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x1f ), result_type::local_failure );
      verify_rule< utf16::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::success );
      verify_rule< utf16::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x7e ), result_type::success );
      verify_rule< utf16::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::local_failure );

      verify_ctrl_enabled< utf16::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf16, 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf16, 0x20, 0x7e >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20 ) );

      verify_rule< utf16::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x1f ), result_type::success );
      verify_rule< utf16::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::local_failure );
      verify_rule< utf16::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x7e ), result_type::local_failure );
      verify_rule< utf16::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf16, 0x20, 0x7e > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf16, 0x20, 0x7e >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20ac ) );

      verify_rule< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x1f ), result_type::local_failure );
      verify_rule< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::success );
      verify_rule< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0xa2 ), result_type::local_failure );
      verify_rule< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::success );
      verify_rule< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s( 0xd801 ) + u16s( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf16, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf16, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0x20ac ) );

      verify_rule< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x1f ), result_type::success );
      verify_rule< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::local_failure );
      verify_rule< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0xa2 ), result_type::success );
      verify_rule< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::local_failure );
      verify_rule< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s( 0xd801 ) + u16s( 0xdc37 ), result_type::local_failure );

      verify_ctrl_enabled< utf16::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf16, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf16, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0xa2 ) );

      verify_rule< utf16::bom >( __LINE__, __FILE__, u16s( 0xfeff ), result_type::success );
      verify_rule< utf16::bom >( __LINE__, __FILE__, u16s( 0xfffe ), result_type::local_failure );

      verify_ctrl_enabled< utf16::bom >( __LINE__, __FILE__, u16s( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16, 0xfeff > >( __LINE__, __FILE__, u16s( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16, 0xfeff >, internal::peek_utf16 > >( __LINE__, __FILE__, u16s( 0xfeff ) );

      verify_rule< utf16::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ) + u16s( 0xd801 ) + u16s( 0xdc37 ) + u16s( 0x20 ), result_type::success, 2 );
      verify_ctrl_enabled< utf16::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ) + u16s( 0xd801 ) + u16s( 0xdc37 ) );
   }

   void test_utf16_be()
   {
      verify_ctrl_enabled< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );

      verify_rule< utf16_be::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, "\xff", result_type::local_failure );

      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 1 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, "  ", result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x00ff ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x0100 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x0fff ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x1000 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd900 ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xde00 ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xfffe ) + " ", result_type::success, 1 );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xffff ) + "  ", result_type::success, 2 );

      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd7ff ) + u16s_be( 0xdfff ), result_type::success, 2 );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdc00 ) + u16s_be( 0xdfff ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0x0020 ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xff20 ), result_type::local_failure );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xdf00 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xdfff ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdbff ) + u16s_be( 0xdc00 ), result_type::success );
      verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdbff ) + u16s_be( 0xdfff ), result_type::success );

      verify_rule< utf16_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< utf16_be::many< 0 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::success, 2 );
      verify_rule< utf16_be::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_be::many< 1 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::success );
      verify_rule< utf16_be::many< 2 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::local_failure );
      verify_rule< utf16_be::many< 2 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ), result_type::success );
      verify_rule< utf16_be::many< 2 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0xd801 ) + u16s_be( 0xdc37 ) + u16s_be( 0x20 ), result_type::success, 2 );

      verify_ctrl_enabled< utf16_be::many< 2 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ) );

      verify_rule< utf16_be::one< 0x20 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::success );
      verify_rule< utf16_be::one< 0x20ac > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::success );
      verify_rule< utf16_be::one< 0x10437 > >( __LINE__, __FILE__, u16s_be( 0xd801 ) + u16s_be( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16_be::one< 0x20ac > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16_be, 0x20ac > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16_be, 0x20ac >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );

      verify_rule< utf16_be::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_be::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::local_failure );
      verify_rule< utf16_be::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0xd801 ) + u16s_be( 0xdc37 ), result_type::local_failure );
      verify_rule< utf16_be::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16_be::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf16_be, 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf16_be, 0x20, 0x10437 >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );

      verify_rule< utf16_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x1f ), result_type::local_failure );
      verify_rule< utf16_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::success );
      verify_rule< utf16_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x7e ), result_type::success );
      verify_rule< utf16_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::local_failure );

      verify_ctrl_enabled< utf16_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf16_be, 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf16_be, 0x20, 0x7e >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20 ) );

      verify_rule< utf16_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x1f ), result_type::success );
      verify_rule< utf16_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::local_failure );
      verify_rule< utf16_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x7e ), result_type::local_failure );
      verify_rule< utf16_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf16_be, 0x20, 0x7e > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf16_be, 0x20, 0x7e >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );

      verify_rule< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x1f ), result_type::local_failure );
      verify_rule< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::success );
      verify_rule< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0xa2 ), result_type::local_failure );
      verify_rule< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::success );
      verify_rule< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0xd801 ) + u16s_be( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf16_be, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf16_be, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0x20ac ) );

      verify_rule< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x1f ), result_type::success );
      verify_rule< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::local_failure );
      verify_rule< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0xa2 ), result_type::success );
      verify_rule< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::local_failure );
      verify_rule< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0xd801 ) + u16s_be( 0xdc37 ), result_type::local_failure );

      verify_ctrl_enabled< utf16_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf16_be, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_be( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf16_be, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0xa2 ) );

      verify_rule< utf16_be::bom >( __LINE__, __FILE__, u16s_be( 0xfeff ), result_type::success );
      verify_rule< utf16_be::bom >( __LINE__, __FILE__, u16s_be( 0xfffe ), result_type::local_failure );

      verify_ctrl_enabled< utf16_be::bom >( __LINE__, __FILE__, u16s_be( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16_be, 0xfeff > >( __LINE__, __FILE__, u16s_be( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16_be, 0xfeff >, internal::peek_utf16_be > >( __LINE__, __FILE__, u16s_be( 0xfeff ) );

      verify_rule< utf16_be::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ) + u16s_be( 0xd801 ) + u16s_be( 0xdc37 ) + u16s_be( 0x20 ), result_type::success, 2 );
      verify_ctrl_enabled< utf16_be::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ) + u16s_be( 0xd801 ) + u16s_be( 0xdc37 ) );
   }

   void test_utf16_le()
   {
      verify_ctrl_enabled< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );

      verify_rule< utf16_le::any >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, "\x01", result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, "\xff", result_type::local_failure );

      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 1 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, "  ", result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x00ff ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x0100 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x0fff ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x1000 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd900 ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xde00 ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xfffe ) + " ", result_type::success, 1 );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xffff ) + "  ", result_type::success, 2 );

      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd7ff ) + u16s_le( 0xdfff ), result_type::success, 2 );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdc00 ) + u16s_le( 0xdfff ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0x0020 ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xff20 ), result_type::local_failure );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xdf00 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xdfff ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdbff ) + u16s_le( 0xdc00 ), result_type::success );
      verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdbff ) + u16s_le( 0xdfff ), result_type::success );

      verify_rule< utf16_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< utf16_le::many< 0 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::success, 2 );
      verify_rule< utf16_le::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_le::many< 1 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::success );
      verify_rule< utf16_le::many< 2 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::local_failure );
      verify_rule< utf16_le::many< 2 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ), result_type::success );
      verify_rule< utf16_le::many< 2 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0xd801 ) + u16s_le( 0xdc37 ) + u16s_le( 0x20 ), result_type::success, 2 );

      verify_ctrl_enabled< utf16_le::many< 2 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ) );

      verify_rule< utf16_le::one< 0x20 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::success );
      verify_rule< utf16_le::one< 0x20ac > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::success );
      verify_rule< utf16_le::one< 0x10437 > >( __LINE__, __FILE__, u16s_le( 0xd801 ) + u16s_le( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16_le::one< 0x20ac > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16_le, 0x20ac > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16_le, 0x20ac >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );

      verify_rule< utf16_le::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< utf16_le::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::local_failure );
      verify_rule< utf16_le::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0xd801 ) + u16s_le( 0xdc37 ), result_type::local_failure );
      verify_rule< utf16_le::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16_le::not_one< 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf16_le, 0x20, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf16_le, 0x20, 0x10437 >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );

      verify_rule< utf16_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x1f ), result_type::local_failure );
      verify_rule< utf16_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::success );
      verify_rule< utf16_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x7e ), result_type::success );
      verify_rule< utf16_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::local_failure );

      verify_ctrl_enabled< utf16_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf16_le, 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf16_le, 0x20, 0x7e >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20 ) );

      verify_rule< utf16_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x1f ), result_type::success );
      verify_rule< utf16_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::local_failure );
      verify_rule< utf16_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x7e ), result_type::local_failure );
      verify_rule< utf16_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::success );

      verify_ctrl_enabled< utf16_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf16_le, 0x20, 0x7e > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf16_le, 0x20, 0x7e >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );

      verify_rule< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x1f ), result_type::local_failure );
      verify_rule< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::success );
      verify_rule< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0xa2 ), result_type::local_failure );
      verify_rule< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::success );
      verify_rule< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0xd801 ) + u16s_le( 0xdc37 ), result_type::success );

      verify_ctrl_enabled< utf16_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf16_le, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf16_le, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0x20ac ) );

      verify_rule< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x1f ), result_type::success );
      verify_rule< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::local_failure );
      verify_rule< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0xa2 ), result_type::success );
      verify_rule< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::local_failure );
      verify_rule< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0xd801 ) + u16s_le( 0xdc37 ), result_type::local_failure );

      verify_ctrl_enabled< utf16_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf16_le, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u16s_le( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf16_le, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0xa2 ) );

      verify_rule< utf16_le::bom >( __LINE__, __FILE__, u16s_le( 0xfeff ), result_type::success );
      verify_rule< utf16_le::bom >( __LINE__, __FILE__, u16s_le( 0xfffe ), result_type::local_failure );

      verify_ctrl_enabled< utf16_le::bom >( __LINE__, __FILE__, u16s_le( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf16_le, 0xfeff > >( __LINE__, __FILE__, u16s_le( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf16_le, 0xfeff >, internal::peek_utf16_le > >( __LINE__, __FILE__, u16s_le( 0xfeff ) );

      verify_rule< utf16_le::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ) + u16s_le( 0xd801 ) + u16s_le( 0xdc37 ) + u16s_le( 0x20 ), result_type::success, 2 );
      verify_ctrl_enabled< utf16_le::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ) + u16s_le( 0xd801 ) + u16s_le( 0xdc37 ) );
   }

   void unit_test()
   {
      test_utf16();
      test_utf16_be();
      test_utf16_le();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
