// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NOT_RANGES_HPP
#define TAO_PEGTL_INTERNAL_NOT_RANGES_HPP

#include "../config.hpp"

#include "dependent_false.hpp"
#include "enable_control.hpp"
#include "match_mode.hpp"
#include "not_one.hpp"
#include "not_range.hpp"
#include "ranges_not_ranges.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct not_ranges
      : terminal< not_ranges< Peek, Cs... >, Peek >,
        ranges_not_ranges< match_mode::invert, Peek, Cs... >
   {};

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct not_ranges< Peek, Lo, Hi >
      : not_range< Peek, Lo, Hi >
   {};

   template< typename Peek, typename Peek::data_t C >
   struct not_ranges< Peek, C >
      : not_one< Peek, C >
   {};

   template< typename Peek >
   struct not_ranges< Peek >
   {
      static_assert( dependent_false< Peek > );
   };

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< not_ranges< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
