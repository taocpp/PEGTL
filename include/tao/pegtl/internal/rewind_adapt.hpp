// Copyright (c) 2023-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REWIND_ADAPT_HPP
#define TAO_PEGTL_INTERNAL_REWIND_ADAPT_HPP

#include "../config.hpp"
#include "../count_position.hpp"
#include "../pointer_position.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   [[nodiscard]] const Data* rewind_adapt( const Data* start, const count_position& c ) noexcept
   {
      return start + c.count;
   }

   template< typename Data >
   [[nodiscard]] const Data* rewind_adapt( const Data* /*unused*/, const pointer_position< Data >& p ) noexcept
   {
      return p.data;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
