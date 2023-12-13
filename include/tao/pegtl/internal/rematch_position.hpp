// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REMATCH_POSITION_HPP
#define TAO_PEGTL_INTERNAL_REMATCH_POSITION_HPP

#include "../config.hpp"
#include "../pointer_position.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Guard, typename Input, typename Data >
   [[nodiscard]] auto rematch_position( Guard&, Input& in, const pointer_position< Data > saved )
   {
      return in.previous_position( saved );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
