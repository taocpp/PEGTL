// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_integer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using pu16b = internal::peek_uint16_be;
      using pu16l = internal::peek_uint16_le;
      using pu32b = internal::peek_uint32_be;
      using pu32l = internal::peek_uint32_le;
      using pu64b = internal::peek_uint64_be;
      using pu64l = internal::peek_uint64_le;

      using input1 = view_input< void, std::uint8_t >;
      using input2 = view_input< void, std::uint16_t >;
      using input4 = view_input< void, std::uint32_t >;
      using input8 = view_input< void, std::uint64_t >;

      static_assert( pu16b::bulk< input1 >() );
      static_assert( pu16l::bulk< input1 >() );
      static_assert( pu32b::bulk< input1 >() );
      static_assert( pu32l::bulk< input1 >() );
      static_assert( pu64b::bulk< input1 >() );
      static_assert( pu64l::bulk< input1 >() );

      static_assert( pu16b::bulk< input2 >() );
      static_assert( pu16l::bulk< input2 >() );
      static_assert( pu32b::bulk< input2 >() );
      static_assert( pu32l::bulk< input2 >() );
      static_assert( pu64b::bulk< input2 >() );
      static_assert( pu64l::bulk< input2 >() );

      static_assert( pu16b::bulk< input4 >() );
      static_assert( pu16l::bulk< input4 >() );
      static_assert( pu32b::bulk< input4 >() );
      static_assert( pu32l::bulk< input4 >() );
      static_assert( pu64b::bulk< input4 >() );
      static_assert( pu64l::bulk< input4 >() );

      static_assert( pu16b::bulk< input8 >() );
      static_assert( pu16l::bulk< input8 >() );
      static_assert( pu32b::bulk< input8 >() );
      static_assert( pu32l::bulk< input8 >() );
      static_assert( pu64b::bulk< input8 >() );
      static_assert( pu64l::bulk< input8 >() );

      static_assert( pu16b::size< input1 >() == 2 );
      static_assert( pu16l::size< input1 >() == 2 );
      static_assert( pu32b::size< input1 >() == 4 );
      static_assert( pu32l::size< input1 >() == 4 );
      static_assert( pu64b::size< input1 >() == 8 );
      static_assert( pu64l::size< input1 >() == 8 );

      static_assert( pu16b::size< input2 >() == 1 );
      static_assert( pu16l::size< input2 >() == 1 );

      static_assert( pu32b::size< input4 >() == 1 );
      static_assert( pu32l::size< input4 >() == 1 );

      static_assert( pu64b::size< input8 >() == 1 );
      static_assert( pu64l::size< input8 >() == 1 );
      {
         std::vector< std::uint8_t > v = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
         input1 in( v );
         {
            const auto pair = pu16b::peek( in, 7 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu32b::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu64b::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu16l::peek( in, 7 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu32l::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu64l::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pu16b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 2 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint16_t( 0x0102 ) );
         } {
            const auto pair = pu32b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 4 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint32_t( 0x01020304 ) );
         } {
            const auto pair = pu64b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 8 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint64_t( 0x0102030405060708 ) );
         } {
            const auto pair = pu16l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 2 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint16_t( 0x0201 ) );
         } {
            const auto pair = pu32l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 4 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint32_t( 0x04030201 ) );
         } {
            const auto pair = pu64l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 8 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == std::uint64_t( 0x0807060504030201 ) );
         }
      } {
         std::vector< std::uint16_t > v = { 0x0102 };
         input2 in( v );
         {
            const auto pair = pu16b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( ( pair.data() == 0x0102 ) || ( pair.data() == 0x0201 ) );
         } {
            const auto pair = pu16l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( ( pair.data() == 0x0102 ) || ( pair.data() == 0x0201 ) );
         }
      } {
         std::vector< std::uint32_t > v = { 0x01020304 };
         input4 in( v );
         {
            const auto pair = pu32b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( ( pair.data() == 0x01020304 ) || ( pair.data() == 0x04030201 ) );
         } {
            const auto pair = pu32l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( ( pair.data() == 0x01020304 ) || ( pair.data() == 0x04030201 ) );
         }
      } {
         std::vector< std::uint64_t > v = { 0x0102030404030201 };
         input8 in( v );
         {
            const auto pair = pu64b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0102030404030201 );
         } {
            const auto pair = pu64l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0102030404030201 );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
