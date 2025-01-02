// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_COMBINE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_COMBINE_TRAITS_HPP

#include <algorithm>
#include <type_traits>

#include "../config.hpp"

#include "math_utility.hpp"
#include "one.hpp"
#include "range.hpp"
#include "ranges.hpp"
#include "tester.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename, typename = void >
   struct combine_traits;

   template< typename Peek, typename Peek::data_t... Ls, typename Peek::data_t... Rs >
   struct combine_traits< tester< one< Peek, Ls... > >, tester< one< Peek, Rs... > > >
   {
      using rule_t = tester< one< Peek, Ls..., Rs... > >;
   };

   template< typename Peek, typename Peek::data_t LL, typename Peek::data_t LH, typename Peek::data_t RL, typename Peek::data_t RH >
   struct combine_traits< tester< range< Peek, LL, LH > >, tester< range< Peek, RL, RH > >, std::enable_if_t< ranges_overlap( LL, LH, RL, RH ) > >
   {
      using rule_t = range< Peek, ( std::min )( LL, RL ), ( std::max )( LH, RH ) >;
   };

   template< typename Peek, typename Peek::data_t LL, typename Peek::data_t LH, typename Peek::data_t RL, typename Peek::data_t RH >
   struct combine_traits< tester< range< Peek, LL, LH > >, tester< range< Peek, RL, RH > >, std::enable_if_t< ranges_disjoint( LL, LH, RL, RH ) > >
   {
      using rule_t = ranges< Peek, LL, LH, RL, RH >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
