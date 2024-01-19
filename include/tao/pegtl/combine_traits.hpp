// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_COMBINE_TRAITS_HPP
#define TAO_PEGTL_COMBINE_TRAITS_HPP

#include <algorithm>
#include <type_traits>

#include "config.hpp"
#include "forward.hpp"

#include "internal/math_utility.hpp"
#include "internal/one.hpp"
#include "internal/range.hpp"
#include "internal/ranges.hpp"
#include "internal/single.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Peek, typename Peek::data_t... Ls, typename Peek::data_t... Rs >
   struct combine_traits< internal::single< internal::one< Peek, Ls... > >, internal::single< internal::one< Peek, Rs... > > >
   {
      using rule_t = internal::single< internal::one< Peek, Ls..., Rs... > >;
   };

   template< typename Peek, typename Peek::data_t LL, typename Peek::data_t LH, typename Peek::data_t RL, typename Peek::data_t RH >
   struct combine_traits< internal::single< internal::range< Peek, LL, LH > >, internal::single< internal::range< Peek, RL, RH > >, std::enable_if_t< internal::ranges_overlap( LL, LH, RL, RH ) > >
   {
      using rule_t = internal::range< Peek, ( std::min )( LL, RL ), ( std::max )( LH, RH ) >;
   };

   template< typename Peek, typename Peek::data_t LL, typename Peek::data_t LH, typename Peek::data_t RL, typename Peek::data_t RH >
   struct combine_traits< internal::single< internal::range< Peek, LL, LH > >, internal::single< internal::range< Peek, RL, RH > >, std::enable_if_t< internal::ranges_disjoint( LL, LH, RL, RH ) > >
   {
      using rule_t = internal::ranges< Peek, LL, LH, RL, RH >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
