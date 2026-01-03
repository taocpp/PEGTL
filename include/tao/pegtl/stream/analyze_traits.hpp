// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_STREAM_ANALYZE_TRAITS_HPP
#define TAO_PEGTL_STREAM_ANALYZE_TRAITS_HPP

#include <cstddef>

#include "../config.hpp"

#include "discard.hpp"
#include "is_stream.hpp"
#include "prefetch.hpp"
#include "require.hpp"

#include "../debug/analyze_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name >
   struct analyze_traits< Name, discard >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, is_stream >
      : analyze_opt_traits<>
   {};

   template< typename Name, std::size_t Num >
   struct analyze_traits< Name, prefetch< Num > >
      : analyze_opt_traits<>
   {};

   template< typename Name, std::size_t Num >
   struct analyze_traits< Name, require< Num > >
      : analyze_opt_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
