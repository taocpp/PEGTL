// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_seven.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using peek7 = internal::peek_seven;

      using input1 = view_input< void, std::uint8_t >;
      using input2 = view_input< void, std::uint16_t >;
      using input4 = view_input< void, std::uint32_t >;

      static_assert( !peek7::bulk< input1 >() );
      static_assert( !peek7::bulk< input2 >() );
      static_assert( !peek7::bulk< input4 >() );

      static_assert( peek7::size< input1 >() == 1 );
      static_assert( peek7::size< input2 >() == 1 );
      static_assert( peek7::size< input4 >() == 1 );
      {
         std::vector< std::uint8_t > v = { 0, 1, 127, 128, 200, 255 };
         input1 in( v );
         {
            const auto pair = peek7::peek( in, 0 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 0 );
         }
         {
            const auto pair = peek7::peek( in, 1 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 1 );
         }
         {
            const auto pair = peek7::peek( in, 2 );
            TAO_PEGTL_TEST_ASSERT( pair );
            TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
            TAO_PEGTL_TEST_ASSERT( pair.data() == 127 );
         }
         {
            const auto pair = peek7::peek( in, 3 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.empty() );
         }
         {
            const auto pair = peek7::peek( in, 4 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.empty() );
         }
         {
            const auto pair = peek7::peek( in, 5 );
            TAO_PEGTL_TEST_ASSERT( !pair );
            TAO_PEGTL_TEST_ASSERT( pair.empty() );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
