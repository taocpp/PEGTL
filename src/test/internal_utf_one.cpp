// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/unicode/internal/peek_utf16.hpp>
#include <tao/pegtl/unicode/internal/peek_utf32.hpp>
#include <tao/pegtl/unicode/internal/utf_one.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_utf8, 'a' >::rule_t, internal::ascii_string< 'a' >::rule_t > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_utf8, 'a', 'b', 'c' >::rule_t, internal::one< internal::peek_utf8, 'a', 'b', 'c' >::rule_t > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_utf8, char32_t( 0x10348 ) >::rule_t, internal::ascii_string< char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ) >::rule_t > );

      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_char, 1 >::rule_t, internal::one< internal::peek_char, 1 >::rule_t > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_char, 1, 2, 3 >::rule_t, internal::one< internal::peek_char, 1, 2, 3 >::rule_t > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_utf32, 1 >::rule_t, internal::one< internal::peek_utf32, 1 >::rule_t > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one< internal::peek_utf32, 1, 2, 3 >::rule_t, internal::one< internal::peek_utf32, 1, 2, 3 >::rule_t > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
