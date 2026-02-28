// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_UNORDERED_HPP
#define TAO_PEGTL_EXTRA_UNORDERED_HPP

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"

#include "internal/unordered.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   struct unordered
      : internal::unordered< false, Rules... >
   {};

   template< typename... Rules >
   struct unordered_partial
      : internal::unordered< true, Rules... >
   {};

   template< typename Name, bool Optional, typename... Rules >
   struct analyze_traits< Name, internal::unordered< Optional, Rules... > >
      : std::conditional_t< Optional, analyze_opt_traits< Rules... >, analyze_seq_traits< Rules... > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
