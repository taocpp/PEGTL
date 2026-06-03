// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TERMINAL_ALIASES_HPP
#define TAO_PEGTL_INTERNAL_TERMINAL_ALIASES_HPP

#include "../config.hpp"

#include "ione_impl.hpp"
#include "match_mode.hpp"
#include "one_impl.hpp"
#include "range_impl.hpp"
#include "ranges_impl.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs > using ione = terminal< match_mode::normal, ione_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_ione = terminal< match_mode::invert, ione_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_one = terminal< match_mode::invert, one_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi > using not_range = terminal< match_mode::invert, range_impl< Peek, Lo, Hi > >;
   template< typename Peek, typename Peek::data_t... Cs > using not_ranges = terminal< match_mode::invert, ranges_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t... Cs > using one = terminal< match_mode::normal, one_impl< Peek, Cs... > >;
   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi > using range = terminal< match_mode::normal, range_impl< Peek, Lo, Hi > >;
   template< typename Peek, typename Peek::data_t... Cs > using ranges = terminal< match_mode::normal, ranges_impl< Peek, Cs... > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
