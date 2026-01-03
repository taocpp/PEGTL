// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_ROTATE_STATES_RIGHT_HPP
#define TAO_PEGTL_CONTROL_ROTATE_STATES_RIGHT_HPP

#include "../config.hpp"

#include "internal/shuffle_states.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< std::size_t N >
      struct rotate_states_right
      {
         template< std::size_t S >
         static constexpr std::size_t count = S;

         template< std::size_t I, std::size_t S >
         static constexpr std::size_t value = ( I + S - N ) % S;
      };

   }  // namespace internal

   template< std::size_t N, typename Base >
   using rotate_states_right_b = internal::shuffle_states< internal::rotate_states_right< N >, Base >;

   template< std::size_t N, template< typename... > class Control, typename Rule >
   using rotate_states_right_r = rotate_states_right_b< N, Control< Rule > >;

   template< std::size_t N, template< typename... > class Control >
   struct rotate_states_right_n
   {
      template< typename Rule >
      using type = rotate_states_right_r< N, Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
