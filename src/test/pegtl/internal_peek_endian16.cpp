// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
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
   template< typename Data >
   void test_integer16_8_big()
   {
      using peek_type = internal::peek_endian< Data, internal::big_endian >;
      using pair_type = internal::data_and_size< Data, std::uint8_t >;
      using input_type = base_input< void, std::byte >;

      static_assert( std::is_same_v< typename peek_type::data_t, Data > );
      static_assert( std::is_same_v< typename peek_type::pair_t, pair_type > );
      static_assert( peek_type::template bulk< input_type >() );
      static_assert( peek_type::template size< input_type >() == 2 );

      const std::vector< std::byte > v = { std::byte( 0 ), std::byte( 12 ), std::byte( 42 ), std::byte( 01 ), std::byte( 200 ), std::byte( 100 ) };

      input_type in( v );

      const std::size_t s = 2;
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 12 ) );
      }
      {
         const auto pair = peek_type::peek( in, 2 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 200 * 256 + 100 ) );
      }
      {
         const auto pair = peek_type::peek( in, 3 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
   }

   template< typename Data >
   void test_integer16_8_little()
   {
      using peek_type = internal::peek_endian< Data, internal::little_endian >;
      using pair_type = internal::data_and_size< Data, std::uint8_t >;
      using input_type = base_input< void, std::byte >;

      static_assert( std::is_same_v< typename peek_type::data_t, Data > );
      static_assert( std::is_same_v< typename peek_type::pair_t, pair_type > );
      static_assert( peek_type::template bulk< input_type >() );
      static_assert( peek_type::template size< input_type >() == 2 );

      const std::vector< std::byte > v = { std::byte( 0 ), std::byte( 12 ), std::byte( 42 ), std::byte( 01 ), std::byte( 200 ), std::byte( 100 ) };

      input_type in( v );

      const std::size_t s = 2;
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 12 * 256 ) );
      }
      {
         const auto pair = peek_type::peek( in, 2 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 100 * 256 + 200 ) );
      }
      {
         const auto pair = peek_type::peek( in, 3 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
   }

   template< typename Data >
   void test_integer16_16_byteswap()
   {
      using peek_type = internal::peek_endian< Data, internal::byteswap_endian >;
      using pair_type = internal::data_and_size< Data, std::uint8_t >;
      using input_type = base_input< void, std::uint16_t >;

      static_assert( std::is_same_v< typename peek_type::data_t, Data > );
      static_assert( std::is_same_v< typename peek_type::pair_t, pair_type > );
      static_assert( peek_type::template bulk< input_type >() );
      static_assert( peek_type::template size< input_type >() == 1 );

      const std::vector< std::uint16_t > v = { std::uint16_t( 65532 ), std::uint16_t( 42 ), std::uint16_t( 40000 ) };

      input_type in( v );

      const std::size_t s = 1;
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 64767 ) );
      }
      {
         const auto pair = peek_type::peek( in, 2 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 16540 ) );
      }
      {
         const auto pair = peek_type::peek( in, 3 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
   }

   template< typename Data >
   void test_integer16_16_identity()
   {
      using peek_type = internal::peek_endian< Data, internal::identity_endian >;
      using pair_type = internal::data_and_size< Data, std::uint8_t >;
      using input_type = base_input< void, std::uint16_t >;

      static_assert( std::is_same_v< typename peek_type::data_t, Data > );
      static_assert( std::is_same_v< typename peek_type::pair_t, pair_type > );
      static_assert( peek_type::template bulk< input_type >() );
      static_assert( peek_type::template size< input_type >() == 1 );

      const std::vector< std::uint16_t > v = { std::uint16_t( 65532 ), std::uint16_t( 42 ), std::uint16_t( 40000 ) };

      input_type in( v );

      const std::size_t s = 1;
      {
         const auto pair = peek_type::peek( in );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 65532 ) );
      }
      {
         const auto pair = peek_type::peek( in, 2 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == s );
         TAO_PEGTL_TEST_ASSERT( pair.data() == Data( 40000 ) );
      }
      {
         const auto pair = peek_type::peek( in, 3 * s );
         static_assert( std::is_same_v< std::decay_t< decltype( pair ) >, pair_type > );
         TAO_PEGTL_TEST_ASSERT( !pair );
         TAO_PEGTL_TEST_ASSERT( pair.size() == 0 );
      }
   }

   void unit_test()
   {
      test_integer16_8_big< std::int16_t >();
      test_integer16_8_little< std::int16_t >();
      test_integer16_8_big< std::uint16_t >();
      test_integer16_8_little< std::uint16_t >();
      test_integer16_16_byteswap< std::int16_t >();
      test_integer16_16_identity< std::int16_t >();
      test_integer16_16_byteswap< std::uint16_t >();
      test_integer16_16_identity< std::uint16_t >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
