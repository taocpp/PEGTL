// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/internal/data_and_size.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< internal::char_and_size, internal::data_and_size< char, std::uint8_t > > );
      static_assert( std::is_same_v< internal::char32_and_size, internal::data_and_size< char32_t, std::uint8_t > > );
      {
         internal::data_and_size< std::int32_t, std::uint16_t > das;
         static_assert( std::is_same_v< decltype( das.size() ), std::uint16_t > );
         static_assert( std::is_same_v< decltype( das.data() ), std::int32_t > );
         TAO_PEGTL_TEST_ASSERT( !das );
         TAO_PEGTL_TEST_ASSERT( das.empty() );
         TAO_PEGTL_TEST_ASSERT( das.size() == 0 );  // NOLINT(readability-container-size-empty)
      }
      {
         internal::data_and_size< std::int32_t, void > das;
         static_assert( std::is_same_v< decltype( das.size() ), std::size_t > );
         static_assert( std::is_same_v< decltype( das.data() ), const std::int32_t& > );
         TAO_PEGTL_TEST_ASSERT( !das );
         TAO_PEGTL_TEST_ASSERT( das.empty() );
         TAO_PEGTL_TEST_ASSERT( das.size() == 0 );  // NOLINT(readability-container-size-empty)
         TAO_PEGTL_TEST_ASSERT( das.pointer() == nullptr );
      }
      {
         internal::data_and_size< std::int32_t, std::uint16_t > das( 10000, 100 );
         static_assert( std::is_same_v< decltype( das.size() ), std::uint16_t > );
         static_assert( std::is_same_v< decltype( das.data() ), std::int32_t > );
         TAO_PEGTL_TEST_ASSERT( das );
         TAO_PEGTL_TEST_ASSERT( !das.empty() );
         TAO_PEGTL_TEST_ASSERT( das.size() == 100 );
         TAO_PEGTL_TEST_ASSERT( das.data() == 10000 );
      }
      {
         const std::int32_t value = 42;
         internal::data_and_size das( &value );
         static_assert( std::is_same_v< decltype( das.size() ), std::size_t > );
         static_assert( std::is_same_v< decltype( das.data() ), const std::int32_t& > );
         TAO_PEGTL_TEST_ASSERT( das );
         TAO_PEGTL_TEST_ASSERT( !das.empty() );
         TAO_PEGTL_TEST_ASSERT( das.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( das.data() == value );
         TAO_PEGTL_TEST_ASSERT( das.pointer() == &value );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
