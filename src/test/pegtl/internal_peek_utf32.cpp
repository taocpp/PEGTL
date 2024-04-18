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
   using p32 = internal::peek_utf32;
   using p32b = internal::peek_utf32_be;
   using p32l = internal::peek_utf32_le;
   using pub = internal::peek_unicode_be;
   using pul = internal::peek_unicode_le;

   using input1 = view_input< void, std::uint8_t >;
   using input4 = view_input< void, std::uint32_t >;

   static_assert( !p32b::bulk< input1 >() );
   static_assert( !p32l::bulk< input1 >() );
   static_assert( !pub::bulk< input1 >() );
   static_assert( !pul::bulk< input1 >() );

   static_assert( !p32b::bulk< input4 >() );
   static_assert( !p32l::bulk< input4 >() );
   static_assert( !pub::bulk< input4 >() );
   static_assert( !pul::bulk< input4 >() );

   template< typename P >
   void failure8t( const std::initializer_list< std::uint8_t >& l )
   {
      std::vector< std::uint8_t > v = l;
      input1 in( v );
      TAO_PEGTL_TEST_ASSERT( !P::peek( in ) );
   }

   void failure32( const std::initializer_list< std::uint32_t >& l )
   {
      std::vector< std::uint32_t > v = l;
      {
         input4 in( v );
         TAO_PEGTL_TEST_ASSERT( !p32::peek( in ) );
         TAO_PEGTL_TEST_ASSERT( !pub::peek( in ) || !pul::peek( in ) );
      }
      {
         std::vector< std::uint8_t > w;
         for( const std::uint32_t u : v ) {
            w.push_back( ( u >> 24 ) & 0xff );
            w.push_back( ( u >> 16 ) & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( u & 0xff );
         }
         input1 in( w );
         TAO_PEGTL_TEST_ASSERT( !p32b::peek( in ) );
      }
      {
         std::vector< std::uint8_t > w;
         for( const std::uint32_t u : v ) {
            w.push_back( u & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( ( u >> 16 ) & 0xff );
            w.push_back( ( u >> 24 ) & 0xff );
         }
         input1 in( w );
         TAO_PEGTL_TEST_ASSERT( !p32l::peek( in ) );
      }
   }

   void success32( const char32_t d, const std::initializer_list< std::uint32_t >& l )
   {
      std::vector< std::uint32_t > v = l;
      {
         input4 in( v );
         const auto pair = p32::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
         TAO_PEGTL_TEST_ASSERT( ( pub::peek( in ).data() == d ) ^ ( pul::peek( in ).data() == d ) );
         TAO_PEGTL_TEST_ASSERT( ( p32b::peek( in ).data() == d ) ^ ( p32l::peek( in ).data() == d ) );
      }
      {
         std::vector< std::uint8_t > w;
         for( const std::uint32_t u : v ) {
            w.push_back( ( u >> 24 ) & 0xff );
            w.push_back( ( u >> 16 ) & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( u & 0xff );
         }
         input1 in( w );
         const auto pair = p32b::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
      }
      {
         std::vector< std::uint8_t > w;
         for( const std::uint32_t u : v ) {
            w.push_back( u & 0xff );
            w.push_back( ( u >> 8 ) & 0xff );
            w.push_back( ( u >> 16 ) & 0xff );
            w.push_back( ( u >> 24 ) & 0xff );
         }
         input1 in( w );
         const auto pair = p32l::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == in.size() );
         TAO_PEGTL_TEST_ASSERT( pair.data() == d );
      }
   }

   void unit_test()
   {
      failure8t< p32 >( { 0x00 } );
      failure8t< p32 >( { 0x00, 0x00 } );
      failure8t< p32 >( { 0x00, 0x00, 0x00 } );
      failure8t< p32 >( { 0x01 } );
      failure8t< p32 >( { 0x01, 0x01 } );
      failure8t< p32 >( { 0x01, 0x01, 0x01 } );
      failure8t< p32b >( { 0x00 } );
      failure8t< p32b >( { 0x00, 0x00 } );
      failure8t< p32b >( { 0x00, 0x00, 0x00 } );
      failure8t< p32b >( { 0x01 } );
      failure8t< p32b >( { 0x01, 0x01 } );
      failure8t< p32b >( { 0x01, 0x01, 0x01 } );
      failure8t< p32l >( { 0x00 } );
      failure8t< p32l >( { 0x00, 0x00 } );
      failure8t< p32l >( { 0x00, 0x00, 0x00 } );
      failure8t< p32l >( { 0x01 } );
      failure8t< p32l >( { 0x01, 0x01 } );
      failure8t< p32l >( { 0x01, 0x01, 0x01 } );

      failure32( { 0xd800 } );
      failure32( { 0xdbff } );
      failure32( { 0xdc00 } );
      failure32( { 0xdfff } );
      failure32( { 0x110000 } );
      failure32( { 0x11223344 } );
      failure32( { 0x80000000 } );
      failure32( { 0xffffffff } );

      success32( 1, { 0x01 } );
      success32( 0x80, { 0x80 } );
      success32( 0x20ac, { 0x20ac } );
      success32( 0xd7ff, { 0xd7ff } );
      success32( 0x100000, { 0x100000 } );
      success32( 0x10ffff, { 0x10ffff } );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
