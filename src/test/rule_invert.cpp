// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/unicode/utf16.hpp>
#include <tao/pegtl/unicode/utf32.hpp>
#include <tao/pegtl/unicode/utf8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< invert< one< 'a' > >, typename not_one< 'a' >::rule_t >();
      verify_meta< invert< not_one< 'a' > >, typename one< 'a' >::rule_t >();
      verify_meta< invert< ione< 'a' > >, typename not_ione< 'a' >::rule_t >();
      verify_meta< invert< not_ione< 'a' > >, typename ione< 'a' >::rule_t >();
      verify_meta< invert< range< 'a', 'z' > >, typename not_range< 'a', 'z' >::rule_t >();
      verify_meta< invert< not_range< 'a', 'z' > >, typename range< 'a', 'z' >::rule_t >();
      verify_meta< invert< ranges< 'a', 'z', '5' > >, typename not_ranges< 'a', 'z', '5' >::rule_t >();
      verify_meta< invert< not_ranges< 'a', 'z', '5' > >, typename ranges< 'a', 'z', '5' >::rule_t >();

      verify_meta< invert< utf8::one< 0x20 > >, internal::not_one< internal::peek_char, ' ' > >();
      verify_meta< invert< utf8::one< 0x20, 0xa2 > >, typename utf8::not_one< 0x20, 0xa2 >::rule_t >();
      verify_meta< invert< utf8::not_one< 0xa2 > >, internal::one< internal::peek_utf8, char32_t( 0xa2 ) > >();
      verify_meta< invert< utf8::not_one< 0x20, 0xa2 > >, typename utf8::one< 0x20, 0xa2 >::rule_t >();
      verify_meta< invert< utf8::range< 0x20, 0x7e > >, typename utf8::not_range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf8::not_range< 0x20, 0x7e > >, typename utf8::range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf8::ranges< 0x20, 0x7e, 0x20ac > >, typename utf8::not_ranges< 0x20, 0x7e, 0x20ac >::rule_t >();
      verify_meta< invert< utf8::not_ranges< 0x20, 0x7e, 0x20ac > >, typename utf8::ranges< 0x20, 0x7e, 0x20ac >::rule_t >();

      verify_meta< invert< utf16::one< 0x20 > >, typename utf16::not_one< 0x20 >::rule_t >();
      verify_meta< invert< utf16::not_one< 0x20 > >, typename utf16::one< 0x20 >::rule_t >();
      verify_meta< invert< utf16::range< 0x20, 0x7e > >, typename utf16::not_range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf16::not_range< 0x20, 0x7e > >, typename utf16::range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf16::ranges< 0x20, 0x7e, 0x20ac > >, typename utf16::not_ranges< 0x20, 0x7e, 0x20ac >::rule_t >();
      verify_meta< invert< utf16::not_ranges< 0x20, 0x7e, 0x20ac > >, typename utf16::ranges< 0x20, 0x7e, 0x20ac >::rule_t >();

      verify_meta< invert< utf32::one< 0x20 > >, typename utf32::not_one< 0x20 >::rule_t >();
      verify_meta< invert< utf32::not_one< 0x20 > >, typename utf32::one< 0x20 >::rule_t >();
      verify_meta< invert< utf32::range< 0x20, 0x7e > >, typename utf32::not_range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf32::not_range< 0x20, 0x7e > >, typename utf32::range< 0x20, 0x7e >::rule_t >();
      verify_meta< invert< utf32::ranges< 0x20, 0x7e, 0x20ac > >, typename utf32::not_ranges< 0x20, 0x7e, 0x20ac >::rule_t >();
      verify_meta< invert< utf32::not_ranges< 0x20, 0x7e, 0x20ac > >, typename utf32::ranges< 0x20, 0x7e, 0x20ac >::rule_t >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
