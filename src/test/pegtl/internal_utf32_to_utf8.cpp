// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/internal/utf32_to_utf8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< char32_t... Cs >
   using u_t = internal::utf32_to_utf8_t< Cs... >;

   void unit_test()
   {
      static_assert( std::is_same_v< u_t< 1 >, internal::ascii_string< 1 > > );
      static_assert( std::is_same_v< u_t< 1, 2, 3 >, internal::ascii_string< 1, 2, 3 > > );
      static_assert( std::is_same_v< u_t< 127 >, internal::ascii_string< 127 > > );
      static_assert( std::is_same_v< u_t< 0xa3 >, internal::ascii_string< char( 0xc2 ), char( 0xa3 ) > > );
      static_assert( std::is_same_v< u_t< 0x418 >, internal::ascii_string< char( 0xd0 ), char( 0x98 ) > > );
      static_assert( std::is_same_v< u_t< 0x939 >, internal::ascii_string< char( 0xe0 ), char( 0xa4 ), char( 0xb9 ) > > );
      static_assert( std::is_same_v< u_t< 0x20ac >, internal::ascii_string< char( 0xe2 ), char( 0x82 ), char( 0xac ) > > );
      static_assert( std::is_same_v< u_t< 0xd55c >, internal::ascii_string< char( 0xed ), char( 0x95 ), char( 0x9c ) > > );
      static_assert( std::is_same_v< u_t< 0x10348 >, internal::ascii_string< char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ) > > );

      static_assert( std::is_same_v< u_t< 1, 0xa3, 2, 0x418, 3, 0x939, 4, 0x20ac, 5, 0xd55c, 6, 0x10348, 7 >, internal::ascii_string< 1, char( 0xc2 ), char( 0xa3 ), 2, char( 0xd0 ), char( 0x98 ), 3, char( 0xe0 ), char( 0xa4 ), char( 0xb9 ), 4, char( 0xe2 ), char( 0x82 ), char( 0xac ), 5, char( 0xed ), char( 0x95 ), char( 0x9c ), 6, char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ), 7 > > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
