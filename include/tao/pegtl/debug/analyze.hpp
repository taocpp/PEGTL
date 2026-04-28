// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_ANALYZE_HPP
#define TAO_PEGTL_DEBUG_ANALYZE_HPP

#include <cstddef>

#include "../config.hpp"

#include "internal/analyze_cycles.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Grammar >
   [[nodiscard]] std::size_t analyze( const int verbose = 1 )
   {
      return internal::analyze_cycles< Grammar >( verbose ).problems();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
