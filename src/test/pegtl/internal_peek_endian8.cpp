// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_endian.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Data, typename Endian >
   void test_integer8()
   {
      using peek_type = internal::peek_endian< Data, Endian >;
      using pair_type = internal::data_and_size< Data, std::uint8_t >;
      using input_type = base_input< void, std::byte >;

      static_assert( std::is_same_v< typename peek_type::data_t, Data > );
      static_assert( std::is_same_v< typename peek_type::pair_t, pair_type > );
      static_assert( peek_type::template bulk< input_type >() );
      static_assert( peek_type::template size< input_type >() == 1 );

      const std::vector< std::byte > v = { std::byte( 0 ), std::byte( 42 ), std::byte( 200 ) };

      input_type in( v );
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 0 ) );
      } {
         const auto pair = peek_type::peek( in, 2 );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 200 ) );
      } {
         const auto pair = peek_type::peek( in, 3 );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
   }

   void unit_test()
   {
      test_integer8< char, internal::big_endian >();
      test_integer8< char, internal::little_endian >();
      test_integer8< char, internal::byteswap_endian >();
      test_integer8< char, internal::identity_endian >();
      test_integer8< std::int8_t, internal::big_endian >();
      test_integer8< std::int8_t, internal::little_endian >();
      test_integer8< std::int8_t, internal::byteswap_endian >();
      test_integer8< std::int8_t, internal::identity_endian >();
      test_integer8< std::uint8_t, internal::big_endian >();
      test_integer8< std::uint8_t, internal::little_endian >();
      test_integer8< std::uint8_t, internal::byteswap_endian >();
      test_integer8< std::uint8_t, internal::identity_endian >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
