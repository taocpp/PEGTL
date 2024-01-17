// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_COMBINE_TRAITS_HPP
#define TAO_PEGTL_COMBINE_TRAITS_HPP

#include "config.hpp"
#include "forward.hpp"

#include "internal/one.hpp"
#include "internal/single.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Peek, typename Peek::data_t... Ls, typename Peek::data_t... Rs >
   struct combine_traits< internal::single< internal::one< Peek, Ls... > >, internal::single< internal::one< Peek, Rs... > > >
   {
      using rule_t = internal::single< internal::one< Peek, Ls..., Rs... > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
