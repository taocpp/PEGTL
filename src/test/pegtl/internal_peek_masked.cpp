// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_masked.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using pm8 = internal::peek_mask_uint8< 0x0f >;
      using pm16b = internal::peek_mask_uint16_be< 0x0ff7 >;
      using pm16l = internal::peek_mask_uint16_le< 0x0ff7 >;
      using pm32b = internal::peek_mask_uint32_be< 0xffff0ff7 >;
      using pm32l = internal::peek_mask_uint32_le< 0xffff0ff7 >;
      using pm64b = internal::peek_mask_uint64_be< 0x0077ff00ffff0ff7 >;
      using pm64l = internal::peek_mask_uint64_le< 0x0077ff00ffff0ff7 >;

      using input1 = view_input< void, std::uint8_t >;
      using input2 = view_input< void, std::uint16_t >;
      using input4 = view_input< void, std::uint32_t >;
      using input8 = view_input< void, std::uint64_t >;

      static_assert( pm8::bulk< input1 >() );
      static_assert( pm16b::bulk< input1 >() );
      static_assert( pm16l::bulk< input1 >() );
      static_assert( pm32b::bulk< input1 >() );
      static_assert( pm32l::bulk< input1 >() );
      static_assert( pm64b::bulk< input1 >() );
      static_assert( pm64l::bulk< input1 >() );

      static_assert( pm16b::bulk< input2 >() );
      static_assert( pm16l::bulk< input2 >() );
      static_assert( pm32b::bulk< input2 >() );
      static_assert( pm32l::bulk< input2 >() );
      static_assert( pm64b::bulk< input2 >() );
      static_assert( pm64l::bulk< input2 >() );

      static_assert( pm16b::bulk< input4 >() );
      static_assert( pm16l::bulk< input4 >() );
      static_assert( pm32b::bulk< input4 >() );
      static_assert( pm32l::bulk< input4 >() );
      static_assert( pm64b::bulk< input4 >() );
      static_assert( pm64l::bulk< input4 >() );

      static_assert( pm16b::bulk< input8 >() );
      static_assert( pm16l::bulk< input8 >() );
      static_assert( pm32b::bulk< input8 >() );
      static_assert( pm32l::bulk< input8 >() );
      static_assert( pm64b::bulk< input8 >() );
      static_assert( pm64l::bulk< input8 >() );

      static_assert( pm8::size< input1 >() == 1 );
      static_assert( pm16b::size< input1 >() == 2 );
      static_assert( pm16l::size< input1 >() == 2 );
      static_assert( pm32b::size< input1 >() == 4 );
      static_assert( pm32l::size< input1 >() == 4 );
      static_assert( pm64b::size< input1 >() == 8 );
      static_assert( pm64l::size< input1 >() == 8 );

      static_assert( pm16b::size< input2 >() == 1 );
      static_assert( pm16l::size< input2 >() == 1 );

      static_assert( pm32b::size< input4 >() == 1 );
      static_assert( pm32l::size< input4 >() == 1 );

      static_assert( pm64b::size< input8 >() == 1 );
      static_assert( pm64l::size< input8 >() == 1 );
      {
         std::vector< std::uint8_t > v = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
         input1 in( v );
         {
            const auto pair = pm16b::peek( in, 7 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm32b::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm64b::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm16l::peek( in, 7 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm32l::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm64l::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( !pair );
         } {
            const auto pair = pm8::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0f );
         } {
            const auto pair = pm16b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 2 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0ff7 );
         } {
            const auto pair = pm32b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 4 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0xffff0ff7 );
         } {
            const auto pair = pm64b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 8 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0077ff00ffff0ff7 );
         } {
            const auto pair = pm16l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 2 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0ff7 );
         } {
            const auto pair = pm32l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 4 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0xffff0ff7 );
         } {
            const auto pair = pm64l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 8 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0077ff00ffff0ff7 );
         }
      } {
         std::vector< std::uint8_t > v = { 0x33 };
         input1 in( v );
         const auto pair = pm8::peek( in );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( pair.data() == 0x03 );
      } {
         std::vector< std::uint16_t > v = { 0xffff };
         input2 in( v );
         {
            const auto pair = pm16b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0ff7 );
         } {
            const auto pair = pm16l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0ff7 );
         }
      } {
         std::vector< std::uint16_t > v = { 0xcccc };
         input2 in( v );
         {
            const auto pair = pm16b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0cc4 );
         } {
            const auto pair = pm16l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0cc4 );
         }
      } {
         std::vector< std::uint32_t > v = { 0xffffffff };
         input4 in( v );
         {
            const auto pair = pm32b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( 0xffff0ff7 );
         } {
            const auto pair = pm32l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( 0xffff0ff7 );
         }
      } {
         std::vector< std::uint64_t > v = { 0xffffffffffffffff };
         input8 in( v );
         {
            const auto pair = pm64b::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0077ff00ffff0ff7 );
         } {
            const auto pair = pm64l::peek( in );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0x0077ff00ffff0ff7 );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
