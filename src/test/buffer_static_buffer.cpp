// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using sb_t = internal::static_buffer< char, internal::endless_reader, 1001, 203 >;

      static_assert( std::is_same_v< char, sb_t::data_t > );

      const std::string st = "hallo";

      sb_t sb( st );

      TAO_PEGTL_TEST_ASSERT( sb.buffer_capacity() == 1001 );
      TAO_PEGTL_TEST_ASSERT( sb.buffer_chunk_size() == 203 );
      TAO_PEGTL_TEST_ASSERT( sb.mutable_start() == sb.buffer_start() );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
