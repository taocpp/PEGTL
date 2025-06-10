// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/internal/utf_one.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_utf8, 'a' >, internal::ascii_string< 'a' > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_utf8, 'a', 'b', 'c' >, internal::one< internal::peek_utf8, 'a', 'b', 'c' > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_utf8, char32_t( 0x10348 ) >, internal::ascii_string< char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ) > > );

      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_char, 1 >, internal::one< internal::peek_char, 1 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_char, 1, 2, 3 >, internal::one< internal::peek_char, 1, 2, 3 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_utf32, 1 >, internal::one< internal::peek_utf32, 1 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::utf_one_t< internal::peek_utf32, 1, 2, 3 >, internal::one< internal::peek_utf32, 1, 2, 3 > > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
