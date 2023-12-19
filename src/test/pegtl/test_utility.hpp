// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_UTILITY_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_UTILITY_HPP

#include <cstddef>

#include <tao/pegtl/eol.hpp>
#include <tao/pegtl/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   template< typename T >
   [[nodiscard]] bool equal( const T& position, const std::size_t count, const std::size_t line, const std::size_t column ) noexcept
   {
      return ( position.count == count ) && ( position.line == line ) && ( position.column == column );
   }

}  // namespace TAO_PEGTL_NAMESPACE::test

#endif
