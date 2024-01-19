// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/unicode.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< unicode::string<>, internal::success >();
      verify_meta< unicode::string< 1 >, internal::single< internal::one< internal::peek_unicode, 1 > > >();
      verify_meta< unicode::string< 1, 2, 3 >, internal::unicode_string< 1, 2, 3 > >();

      verify_analyze< unicode::string< 1, 2, 3 > >( __LINE__, __FILE__, true, false );

      static_assert( std::is_same_v< unicode::string<>::rule_t, internal::success::rule_t > );
      static_assert( std::is_same_v< unicode::string< 1 >::rule_t, internal::one< internal::peek_unicode, char32_t( 1 ) >::rule_t > );
      {
         std::vector< char > v = { 1, char( 0xc2 ), char( 0xa3 ), 2, char( 0xd0 ), char( 0x98 ), 3, char( 0xe0 ), char( 0xa4 ), char( 0xb9 ), 4, char( 0xe2 ), char( 0x82 ), char( 0xac ), 5, char( 0xed ), char( 0x95 ), char( 0x9c ), 6, char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ), 7 };
         view_input< eols::scan::lf > in( v );
         TAO_PEGTL_TEST_ASSERT( parse< unicode::string< 1, 0xa3, 2, 0x418, 3, 0x939, 4, 0x20ac, 5, 0xd55c, 6, 0x10348, 7 > >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      } {
         std::vector< std::uint16_t > v = { 0x20, 0x20ac, 0xd801, 0xdc37 };
         view_input< eols::scan::lf, std::uint16_t > in( v );
         TAO_PEGTL_TEST_ASSERT( parse< unicode::string< 0x20, 0x20ac, 0x10437 > >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      } {
         std::vector< char32_t > v = { 0x20, 0x20ac, 0x10437 };
         view_input< eols::scan::lf, char32_t > in( v );
         TAO_PEGTL_TEST_ASSERT( parse< unicode::string< 0x20, 0x20ac, 0x10437 > >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
