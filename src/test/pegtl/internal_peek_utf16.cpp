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
   using p16 = internal::peek_utf16;
   using p16b = internal::peek_utf16_be;
   using p16l = internal::peek_utf16_le;
   using pub = internal::peek_unicode_be;
   using pul = internal::peek_unicode_le;

   using input1 = view_input< void, std::uint8_t >;
   using input2 = view_input< void, std::uint16_t >;

   static_assert( !p16b::bulk< input1 >() );
   static_assert( !p16l::bulk< input1 >() );
   static_assert( !pub::bulk< input1 >() );
   static_assert( !pul::bulk< input1 >() );

   static_assert( !p16b::bulk< input2 >() );
   static_assert( !p16l::bulk< input2 >() );
   static_assert( !pub::bulk< input2 >() );
   static_assert( !pul::bulk< input2 >() );

   template< typename P >
   void failure8t( const std::initializer_list< std::uint8_t >& l )
   {
      std::vector< std::uint8_t > v = l;
      input1 in( v );
      TAO_PEGTL_TEST_ASSERT( !P::peek( in ) );
   }

   void failure16( const std::initializer_list< std::uint16_t >& l )
   {
      std::vector< std::uint16_t > v = l;
      {
         input2 in( v );
         TAO_PEGTL_TEST_ASSERT( !p16::peek( in ) );
         TAO_PEGTL_TEST_ASSERT( !pub::peek( in ) || !pul::peek( in ) );
      } {
         std::vector< std::uint8_t > w;
         for( const std::uint16_t u : v ) {
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( u & 0xff );
         }
         input1 in( w );
         TAO_PEGTL_TEST_ASSERT( !p16b::peek( in ) );
      } {
         std::vector< std::uint8_t > w;
         for( const std::uint16_t u : v ) {
            w.push_back( u & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
         }
         input1 in( w );
         TAO_PEGTL_TEST_ASSERT( !p16l::peek( in ) );
      }
   }

   void success16( const char32_t d, const std::initializer_list< std::uint16_t >& l )
   {
      std::vector< std::uint16_t > v = l;
      {
         input2 in( v );
         const auto pair = p16::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
         TAO_PEGTL_TEST_ASSERT( ( pub::peek( in ).data() == d ) ^ ( pul::peek( in ).data() == d ) );
         TAO_PEGTL_TEST_ASSERT( ( p16b::peek( in ).data() == d ) ^ ( p16l::peek( in ).data() == d ) );
      } {
         std::vector< std::uint8_t > w;
         for( const std::uint16_t u : v ) {
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( u & 0xff );
         }
         input1 in( w );
         const auto pair = p16b::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
      } {
         std::vector< std::uint8_t > w;
         for( const std::uint16_t u : v ) {
            w.push_back( u & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
         }
         input1 in( w );
         const auto pair = p16l::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
      }
   }

   void unit_test()
   {
      failure8t< p16 >( { 0x00 } );
      failure8t< p16 >( { 0x17 } );
      failure8t< p16 >( { 0x80 } );
      failure8t< p16 >( { 0xff } );
      failure8t< p16b >( { 0x00 } );
      failure8t< p16b >( { 0x17 } );
      failure8t< p16b >( { 0x80 } );
      failure8t< p16b >( { 0xff } );
      failure8t< p16l >( { 0x00 } );
      failure8t< p16l >( { 0x17 } );
      failure8t< p16l >( { 0x80 } );
      failure8t< p16l >( { 0xff } );

      failure16( { 0xd800 } );
      failure16( { 0xdbff } );
      failure16( { 0xdc00 } );
      failure16( { 0xdfff } );
      failure16( { 0xd800, 0xd800 } );
      failure16( { 0xdc00, 0xdc00 } );
      failure16( { 0xdc00, 0xd800 } );

      success16( 0x01, { 0x0001 } );
      success16( 0x7f, { 0x007f } );
      success16( 0x20ac, { 0x20ac } );
      success16( 0x10437, { 0xd801, 0xdc37 } );
      success16( 0x24b62, { 0xd852, 0xdf62 } );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
