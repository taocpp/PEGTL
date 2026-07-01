// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_endian.hpp"
#include "verify_ctrl.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/unicode/utf32.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace
   {
      [[nodiscard]] std::string u32s( const char32_t u )
      {
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

      [[nodiscard]] std::string u32s_be( const char32_t v )
      {
         const std::uint32_t u = test::h_to_be( static_cast< std::uint32_t >( v ) );
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

      [[nodiscard]] std::string u32s_le( const char32_t v )
      {
         const std::uint32_t u = test::h_to_le( static_cast< std::uint32_t >( v ) );
         return { reinterpret_cast< const char* >( &u ), sizeof( u ) };
      }

   }  // namespace

   void test_utf32()
   {
      verify_ctrl_enabled< utf32::any >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20ac ) );

      verify_rule< utf32::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, "\xff", result_type::local_failure, 1 );
      verify_rule< utf32::any >( __LINE__, __FILE__, "\xff\xff", result_type::local_failure, 2 );
      verify_rule< utf32::any >( __LINE__, __FILE__, "\xff\xff\xff", result_type::local_failure, 3 );

      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0 ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 1 ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x00ff ) + " ", result_type::success, 1 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x0100 ) + "  ", result_type::success, 2 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x0fff ) + "   ", result_type::success, 3 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x1000 ) + "    ", result_type::success, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xd7ff ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xe000 ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xfffe ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xffff ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x100000 ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x10fffe ), result_type::success, 0 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x10ffff ), result_type::success, 0 );

      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xd800 ), result_type::local_failure, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xd900 ), result_type::local_failure, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xdc00 ), result_type::local_failure, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0xdfff ), result_type::local_failure, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x110000 ), result_type::local_failure, 4 );
      verify_rule< utf32::any >( __LINE__, __FILE__, u32s( 0x110000 ) + u32s( 0 ), result_type::local_failure, 8 );

      verify_rule< utf32::many< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< utf32::many< 0 > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::success, 4 );
      verify_rule< utf32::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32::many< 1 > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::success, 0 );
      verify_rule< utf32::many< 2 > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32::many< 2 > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32::many< 2 > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ) + u32s( 0x10fedc ), result_type::success, 4 );

      verify_ctrl_enabled< utf32::many< 2 > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ) );

      verify_rule< utf32::one< 0x20 > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::success, 0 );
      verify_rule< utf32::one< 0x20ac > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32::one< 0x10fedc > >( __LINE__, __FILE__, u32s( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32::one< 0x20ac > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32, 0x20ac > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32, 0x20ac >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20ac ) );

      verify_rule< utf32::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x10fedc ), result_type::local_failure, 4 );
      verify_rule< utf32::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf32, 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf32, 0x20, 0x10fedc >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20ac ) );

      verify_rule< utf32::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::success, 0 );
      verify_rule< utf32::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x7e ), result_type::success, 0 );
      verify_rule< utf32::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf32, 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf32, 0x20, 0x7e >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20 ) );

      verify_rule< utf32::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x1f ), result_type::success, 0 );
      verify_rule< utf32::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x7e ), result_type::local_failure, 4 );
      verify_rule< utf32::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf32, 0x20, 0x7e > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf32, 0x20, 0x7e >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20ac ) );

      verify_rule< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::success, 0 );
      verify_rule< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0xa2 ), result_type::local_failure, 4 );
      verify_rule< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf32, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf32, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0x20ac ) );

      verify_rule< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x1f ), result_type::success, 0 );
      verify_rule< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0xa2 ), result_type::success, 0 );
      verify_rule< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0x20ac ), result_type::local_failure, 4 );
      verify_rule< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x10fedc ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf32, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf32, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0xa2 ) );

      verify_ctrl_enabled< utf32::bom >( __LINE__, __FILE__, u32s( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32, 0xfeff > >( __LINE__, __FILE__, u32s( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32, 0xfeff >, internal::peek_utf32 > >( __LINE__, __FILE__, u32s( 0xfeff ) );

      verify_rule< utf32::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ) + u32s( 0x10fedc ) + u32s( 0x20 ), result_type::success, 4 );
      verify_ctrl_enabled< utf32::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s( 0x20 ) + u32s( 0x20ac ) + u32s( 0x10fedc ) );
   }

   void test_utf32_be()
   {
      verify_ctrl_enabled< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );

      verify_rule< utf32_be::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, "\xff", result_type::local_failure, 1 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, "\xff\xff", result_type::local_failure, 2 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, "\xff\xff\xff", result_type::local_failure, 3 );

      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0 ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 1 ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x00ff ) + " ", result_type::success, 1 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x0100 ) + "  ", result_type::success, 2 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x0fff ) + "   ", result_type::success, 3 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x1000 ) + "    ", result_type::success, 4 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0xfffe ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0xffff ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x100000 ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x10fffe ), result_type::success, 0 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x10ffff ), result_type::success, 0 );

      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x110000 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::any >( __LINE__, __FILE__, u32s_be( 0x110000 ) + u32s_be( 0 ), result_type::local_failure, 8 );

      verify_rule< utf32_be::many< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< utf32_be::many< 0 > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::success, 4 );
      verify_rule< utf32_be::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_be::many< 1 > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_be::many< 2 > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::many< 2 > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_be::many< 2 > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ) + u32s_be( 0x10fedc ), result_type::success, 4 );

      verify_ctrl_enabled< utf32_be::many< 2 > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ) );

      verify_rule< utf32_be::one< 0x20 > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_be::one< 0x20ac > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_be::one< 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_be::one< 0x20ac > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32_be, 0x20ac > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32_be, 0x20ac >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );

      verify_rule< utf32_be::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_be::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x10fedc ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_be::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf32_be, 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf32_be, 0x20, 0x10fedc >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );

      verify_rule< utf32_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x7e ), result_type::success, 0 );
      verify_rule< utf32_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32_be::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf32_be, 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf32_be, 0x20, 0x7e >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20 ) );

      verify_rule< utf32_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x1f ), result_type::success, 0 );
      verify_rule< utf32_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x7e ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_be::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf32_be, 0x20, 0x7e > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf32_be, 0x20, 0x7e >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );

      verify_rule< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0xa2 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_be::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf32_be, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf32_be, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0x20ac ) );

      verify_rule< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x1f ), result_type::success, 0 );
      verify_rule< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0xa2 ), result_type::success, 0 );
      verify_rule< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0x20ac ), result_type::local_failure, 4 );
      verify_rule< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x10fedc ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32_be::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf32_be, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_be( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf32_be, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0xa2 ) );

      verify_ctrl_enabled< utf32_be::bom >( __LINE__, __FILE__, u32s_be( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32_be, 0xfeff > >( __LINE__, __FILE__, u32s_be( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32_be, 0xfeff >, internal::peek_utf32_be > >( __LINE__, __FILE__, u32s_be( 0xfeff ) );

      verify_rule< utf32_be::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ) + u32s_be( 0x10fedc ) + u32s_be( 0x20 ), result_type::success, 4 );
      verify_ctrl_enabled< utf32_be::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s_be( 0x20 ) + u32s_be( 0x20ac ) + u32s_be( 0x10fedc ) );
   }

   void test_utf32_le()
   {
      verify_ctrl_enabled< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::any< internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );

      verify_rule< utf32_le::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, "\xff", result_type::local_failure, 1 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, "\xff\xff", result_type::local_failure, 2 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, "\xff\xff\xff", result_type::local_failure, 3 );

      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0 ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 1 ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x00ff ) + " ", result_type::success, 1 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x0100 ) + "  ", result_type::success, 2 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x0fff ) + "   ", result_type::success, 3 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x1000 ) + "    ", result_type::success, 4 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0xfffe ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0xffff ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x100000 ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x10fffe ), result_type::success, 0 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x10ffff ), result_type::success, 0 );

      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x110000 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::any >( __LINE__, __FILE__, u32s_le( 0x110000 ) + u32s_le( 0 ), result_type::local_failure, 8 );

      verify_rule< utf32_le::many< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< utf32_le::many< 0 > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::success, 4 );
      verify_rule< utf32_le::many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_le::many< 1 > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_le::many< 2 > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::many< 2 > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_le::many< 2 > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ) + u32s_le( 0x10fedc ), result_type::success, 4 );

      verify_ctrl_enabled< utf32_le::many< 2 > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::many< 2, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ) );

      verify_rule< utf32_le::one< 0x20 > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_le::one< 0x20ac > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_le::one< 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_le::one< 0x20ac > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32_le, 0x20ac > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32_le, 0x20ac >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );

      verify_rule< utf32_le::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf32_le::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x10fedc ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_le::not_one< 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::not_one< internal::peek_utf32_le, 0x20, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_one< internal::peek_utf32_le, 0x20, 0x10fedc >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );

      verify_rule< utf32_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x7e ), result_type::success, 0 );
      verify_rule< utf32_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32_le::range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20 ) );
      verify_ctrl_disabled< internal::range< internal::peek_utf32_le, 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20 ) );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_utf32_le, 0x20, 0x7e >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20 ) );

      verify_rule< utf32_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x1f ), result_type::success, 0 );
      verify_rule< utf32_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x7e ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_le::not_range< 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::not_range< internal::peek_utf32_le, 0x20, 0x7e > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::not_range< internal::peek_utf32_le, 0x20, 0x7e >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );

      verify_rule< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x1f ), result_type::local_failure, 4 );
      verify_rule< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::success, 0 );
      verify_rule< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0xa2 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::success, 0 );
      verify_rule< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x10fedc ), result_type::success, 0 );

      verify_ctrl_enabled< utf32_le::ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::ranges< internal::peek_utf32_le, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );
      verify_ctrl_disabled< internal::terminal< internal::ranges< internal::peek_utf32_le, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0x20ac ) );

      verify_rule< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x1f ), result_type::success, 0 );
      verify_rule< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20 ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0xa2 ), result_type::success, 0 );
      verify_rule< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0x20ac ), result_type::local_failure, 4 );
      verify_rule< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x10fedc ), result_type::local_failure, 4 );

      verify_ctrl_enabled< utf32_le::not_ranges< 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0xa2 ) );
      verify_ctrl_disabled< internal::not_ranges< internal::peek_utf32_le, 0x20, 0x7e, 0x20ac, 0x20af > >( __LINE__, __FILE__, u32s_le( 0xa2 ) );
      verify_ctrl_disabled< internal::terminal< internal::not_ranges< internal::peek_utf32_le, 0x20, 0x7e, 0x20ac, 0x20af >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0xa2 ) );

      verify_ctrl_enabled< utf32_le::bom >( __LINE__, __FILE__, u32s_le( 0xfeff ) );
      verify_ctrl_disabled< internal::one< internal::peek_utf32_le, 0xfeff > >( __LINE__, __FILE__, u32s_le( 0xfeff ) );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_utf32_le, 0xfeff >, internal::peek_utf32_le > >( __LINE__, __FILE__, u32s_le( 0xfeff ) );

      verify_rule< utf32_le::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ) + u32s_le( 0x10fedc ) + u32s_le( 0x20 ), result_type::success, 4 );
      verify_ctrl_enabled< utf32_le::string< 0x20, 0x20ac, 0x10fedc > >( __LINE__, __FILE__, u32s_le( 0x20 ) + u32s_le( 0x20ac ) + u32s_le( 0x10fedc ) );
   }

   void unit_test()
   {
      test_utf32();
      test_utf32_be();
      test_utf32_le();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
