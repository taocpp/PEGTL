// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REVERSE_STATES_HPP
#define TAO_PEGTL_CONTROL_REVERSE_STATES_HPP

#include "../config.hpp"

#include "internal/shuffle_states.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct reverse_states
      {
         template< std::size_t S >
         static constexpr std::size_t count = S;

         template< std::size_t I, std::size_t S >
         static constexpr std::size_t value = ( S - 1 ) - I;
      };

   }  // namespace internal

   template< typename Base >
   using reverse_states_b = internal::shuffle_states< internal::reverse_states, Base >;

   template< template< typename... > class Control, typename Rule >
   using reverse_states_r = internal::shuffle_states< internal::reverse_states, Control< Rule > >;

   template< template< typename... > class Control >
   struct reverse_states_n
   {
      template< typename Rule >
      using type = internal::shuffle_states< internal::reverse_states, Control< Rule > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
