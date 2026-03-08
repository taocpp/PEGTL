// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TERMINAL_ALIASES_HPP
#define TAO_PEGTL_INTERNAL_TERMINAL_ALIASES_HPP

#include "../config.hpp"

#include "invert_mode.hpp"
#include "ione_impl.hpp"
#include "one_impl.hpp"
#include "range_impl.hpp"
#include "ranges_impl.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs > using ione = terminal< invert_mode::disabled, ione_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_ione = terminal< invert_mode::enabled, ione_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_one = terminal< invert_mode::enabled, one_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi > using not_range = terminal< invert_mode::enabled, range_impl< Peek, Lo, Hi > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_ranges = terminal< invert_mode::enabled, ranges_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using one = terminal< invert_mode::disabled, one_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi > using range = terminal< invert_mode::disabled, range_impl< Peek, Lo, Hi > >;
   template< typename Peek, typename Peek::data_t... Cs > using ranges = terminal< invert_mode::disabled, ranges_impl< Peek, Cs... > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
