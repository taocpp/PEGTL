// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_CURRENT_HPP
#define TAO_PEGTL_INTERNAL_PEEK_CURRENT_HPP

#include <cstddef>
#include <cstdint>

#include "data_and_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct peek_current
   {
      // using data_t = Depends on ParseInput.
      // using pair_t = Depends on ParseInput.

      static constexpr bool allow_bulk = true;
      static constexpr std::size_t fixed_size = 1;

      template< typename ParseInput >
      [[nodiscard]] static auto peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? in.current( offset ) : nullptr );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
