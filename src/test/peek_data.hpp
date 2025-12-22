// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEEK_DATA_HPP
#define TAO_PEGTL_SRC_TEST_PEEK_DATA_HPP

#include <cstddef>
#include <cstdint>

#include <tao/pegtl/internal/data_and_size.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   struct peek_data
   {
      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static auto peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         return internal::data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? in.current( offset ) : nullptr );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::test

#endif
