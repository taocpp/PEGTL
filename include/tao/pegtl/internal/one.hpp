// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ONE_HPP
#define TAO_PEGTL_INTERNAL_ONE_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "match_mode.hpp"
#include "one_not_one.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct one
      : terminal< one< Peek, Cs... >, Peek >,
        one_not_one< match_mode::normal, Peek, Cs... >
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< one< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
