// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/utf8_append.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::string s;
      internal::utf8_append_1_impl( s, 0x42 );
      TAO_PEGTL_TEST_ASSERT( s == "\x42" );
      s.clear();
      internal::utf8_append_2_impl( s, 0x375 );
      TAO_PEGTL_TEST_ASSERT( s == "\xcd\xb5" );
      s.clear();
      internal::utf8_append_3_impl( s, 0xb72 );
      TAO_PEGTL_TEST_ASSERT( s == "\xe0\xad\xb2" );
      s.clear();
      internal::utf8_append_4_impl( s, 0x10c48 );
      TAO_PEGTL_TEST_ASSERT( s == "\xf0\x90\xb1\x88" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf16( s, 0x23 ) );
      TAO_PEGTL_TEST_ASSERT( s == "\x23" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf16( s, 0x64e ) );
      TAO_PEGTL_TEST_ASSERT( s == "\xd9\x8e" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf16( s, 0xfb1d ) );
      TAO_PEGTL_TEST_ASSERT( s == "\xef\xac\x9d" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( !internal::utf8_append_utf16( s, 0xdfed ) );
      TAO_PEGTL_TEST_ASSERT( !internal::utf8_append_utf16( s, 0xd800 ) );
      TAO_PEGTL_TEST_ASSERT( internal::utf16_compose( 0xd852, 0xdf62 ) == 0x24b62 );
      internal::utf8_append_utf16( s, 0xd852, 0xdf62 );
      std::string t;
      internal::utf8_append_4_impl( t, 0x24b62 );
      TAO_PEGTL_TEST_ASSERT( s == t );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf32( s, 0x64 ) );
      TAO_PEGTL_TEST_ASSERT( s == "\x64" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf32( s, 0x64e ) );
      TAO_PEGTL_TEST_ASSERT( s == "\xd9\x8e" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf32( s, 0xfb1d ) );
      TAO_PEGTL_TEST_ASSERT( s == "\xef\xac\x9d" );
      s.clear();
      TAO_PEGTL_TEST_ASSERT( !internal::utf8_append_utf32( s, 0xdfed ) );
      TAO_PEGTL_TEST_ASSERT( !internal::utf8_append_utf32( s, 0xd800 ) );
      TAO_PEGTL_TEST_ASSERT( internal::utf8_append_utf32( s, 0x24b62 ) );
      TAO_PEGTL_TEST_ASSERT( s == t );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
