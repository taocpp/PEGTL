// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/internal/char_string.hpp>
#include <tao/pegtl/internal/unicode_to_utf8_char.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< char32_t Char >
   using u_t = typename internal::unicode_to_utf8_char< internal::char_string, Char >::type;

   void unit_test()
   {
      static_assert( std::is_same_v< u_t< 1 >, internal::char_string< 1 > > );
      static_assert( std::is_same_v< u_t< 127 >, internal::char_string< 127 > > );
      static_assert( std::is_same_v< u_t< 0xa3 >, internal::char_string< char( 0xc2 ), char( 0xa3 ) > > );
      static_assert( std::is_same_v< u_t< 0x418 >, internal::char_string< char( 0xd0 ), char( 0x98 ) > > );
      static_assert( std::is_same_v< u_t< 0x939 >, internal::char_string< char( 0xe0 ), char( 0xa4 ), char( 0xb9 ) > > );
      static_assert( std::is_same_v< u_t< 0x20ac >, internal::char_string< char( 0xe2 ), char( 0x82 ), char( 0xac ) > > );
      static_assert( std::is_same_v< u_t< 0xd55c >, internal::char_string< char( 0xed ), char( 0x95 ), char( 0x9c ) > > );
      static_assert( std::is_same_v< u_t< 0x10348 >, internal::char_string< char( 0xf0 ), char( 0x90 ), char( 0x8d ), char( 0x88 ) > > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
