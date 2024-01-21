// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_ascii7.hpp>
#include <tao/pegtl/internal/peek_ascii8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using peek7 = internal::peek_ascii7;
      using peek8 = internal::peek_ascii8;

      using input1 = view_input< void, std::uint8_t >;
      using input2 = view_input< void, std::uint16_t >;
      using input4 = view_input< void, std::uint32_t >;

      static_assert( !peek7::bulk< input1 >() );
      static_assert( peek8::bulk< input1 >() );
      static_assert( !peek7::bulk< input2 >() );
      static_assert( !peek8::bulk< input2 >() );
      static_assert( !peek7::bulk< input4 >() );
      static_assert( !peek8::bulk< input4 >() );

      static_assert( peek7::size< input1 >() == 1 );
      static_assert( peek8::size< input1 >() == 1 );
      static_assert( peek7::size< input2 >() == 1 );
      static_assert( peek8::size< input2 >() == 1 );
      static_assert( peek7::size< input4 >() == 1 );
      static_assert( peek8::size< input4 >() == 1 );
      {
         std::vector< std::uint8_t > v = { 0, 1, 127, 128, 200, 255 };
         input1 in( v );
         {
            const auto pair = peek7::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek7::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek7::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek7::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek8::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek8::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek8::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek8::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 128 ) );
         } {
            const auto pair = peek8::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 200 ) );
         } {
            const auto pair = peek8::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 255 ) );
         }
      } {
         std::vector< std::uint16_t > v = { 0, 1, 127, 128, 200, 400 };
         input2 in( v );
         {
            const auto pair = peek7::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek7::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek7::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek7::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek8::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek8::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek8::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek8::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 128 ) );
         } {
            const auto pair = peek8::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 200 ) );
         } {
            const auto pair = peek8::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         }
      } {
         std::vector< std::uint32_t > v = { 0, 1, 127, 128, 200, 400, 800000 };
         input4 in( v );
         {
            const auto pair = peek7::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek7::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek7::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek7::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek7::peek( in, 6 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek8::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         } {
            const auto pair = peek8::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         } {
            const auto pair = peek8::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         } {
            const auto pair = peek8::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 128 ) );
         } {
            const auto pair = peek8::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == char( 200 ) );
         } {
            const auto pair = peek8::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         } {
            const auto pair = peek8::peek( in, 6 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
