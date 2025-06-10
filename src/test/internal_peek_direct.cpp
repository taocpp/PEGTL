// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_direct.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using peek_type = internal::peek_direct< int >;
      using input_type = base_input< void, int >;

      static_assert( std::is_same_v< peek_type::data_t, int > );
      static_assert( std::is_same_v< peek_type::pair_t, internal::data_and_size< int, void > > );
      static_assert( peek_type::bulk< input_type >() );
      static_assert( peek_type::size< input_type >() == 1 );
      const std::vector< int > v = { 1000, 2000, 3000 };
      input_type in( v );
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, peek_type::pair_t > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( pair.data() == 1000 );
      }
      {
         const auto pair = peek_type::peek( in, 2 );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, peek_type::pair_t > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( pair.data() == 3000 );
      }
      {
         const auto pair = peek_type::peek( in, 3 );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, peek_type::pair_t > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
      // TEST: That peek_direct with a size mismatch doesn't compile.
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
