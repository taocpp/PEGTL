// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_SHUFFLE_STATES_HPP
#define TAO_PEGTL_CONTROL_SHUFFLE_STATES_HPP

#include "../config.hpp"

#include "internal/shuffle_states.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Shuffle, typename Base >
   using shuffle_states_b = internal::shuffle_states< Shuffle, Base >;

   template< typename Shuffle, template< typename... > class Control, typename Rule >
   using shuffle_states_r = shuffle_states_b< Shuffle, Control< Rule > >;

   template< typename Shuffle, template< typename... > class Control >
   struct shuffle_states_n
   {
      template< typename Rule >
      using type = shuffle_states_r< Shuffle, Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
