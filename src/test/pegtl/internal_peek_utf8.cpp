// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_unicode.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using p8 = internal::peek_utf8;
   using pub = internal::peek_unicode_be;
   using pul = internal::peek_unicode_le;

   using input1 = view_input< void, std::uint8_t >;

   static_assert( !p8::bulk< input1 >() );
   static_assert( !pub::bulk< input1 >() );
   static_assert( !pul::bulk< input1 >() );

   template< typename P >
   void failure8t( const std::initializer_list< std::uint8_t >& l )
   {
      std::vector< std::uint8_t > v = l;
      input1 in( v );
      TAO_PEGTL_TEST_ASSERT( !P::peek( in ) );
   }

   template< typename P >
   void success8t( const char32_t d, const std::initializer_list< std::uint8_t >& l )
   {
      std::vector< std::uint8_t > v = l;
      input1 in( v );
      const auto pair = P::peek( in );
      TAO_PEGTL_TEST_ASSERT( pair );
      TAO_PEGTL_TEST_ASSERT( pair.size() == l.size() );
      TAO_PEGTL_TEST_ASSERT( pair.data() == d );
   }

   void failure8( const std::initializer_list< std::uint8_t >& l )
   {
      failure8t< p8 >( l );
      failure8t< pub >( l );
      failure8t< pul >( l );
   }

   void success8( const char32_t d, const std::initializer_list< std::uint8_t >& l )
   {
      success8t< p8 >( d, l );
      success8t< pub >( d, l );
      success8t< pul >( d, l );
   }

   void unit_test()
   {
      failure8( { 0x80 } );
      failure8( { 0x80, 0x80 } );
      failure8( { 0x80, 0x80, 0x80 } );
      failure8( { 0x80, 0x80, 0x80, 0x80 } );
      failure8( { 0xc0 } );
      failure8( { 0xc0, 0x00 } );
      failure8( { 0xc0, 0x7f } );
      failure8( { 0xe0 } );
      failure8( { 0xe0, 0x80 } );
      failure8( { 0xe0, 0x00, 0x80 } );
      failure8( { 0xe0, 0x80, 0x7f } );
      failure8( { 0xf0 } );
      failure8( { 0xf8 } );
      failure8( { 0xf0, 0x80 } );
      failure8( { 0xf0, 0x80, 0x80 } );
      failure8( { 0xf0, 0x80, 0x80, 0x7f } );
      success8( 0x00, { 0x00 } );
      success8( 0x7f, { 0x7f } );
      success8( 0xa3, { 0xc2, 0xa3 } );
      success8( 0x418, { 0xd0, 0x98 } );
      success8( 0x939, { 0xe0, 0xa4, 0xb9 } );
      success8( 0x20ac, { 0xe2, 0x82, 0xac } );
      success8( 0xd55c, { 0xed, 0x95, 0x9c } );
      success8( 0x10348, { 0xf0, 0x90, 0x8d, 0x88 } );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
