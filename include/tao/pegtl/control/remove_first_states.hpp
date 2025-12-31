// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REMOVE_FIRST_STATES_HPP
#define TAO_PEGTL_CONTROL_REMOVE_FIRST_STATES_HPP

#include "../config.hpp"

#include "internal/shuffle_states.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< std::size_t N, std::size_t S >
      struct remove_first_states_count
      {
         static_assert( S >= N );

         static constexpr std::size_t count = S - N;
      };

      template< std::size_t N >
      struct remove_first_states
      {
         template< std::size_t S >
         static constexpr std::size_t count = remove_first_states_count< N, S >::count;

         template< std::size_t I, std::size_t S >
         static constexpr std::size_t value = I + N;
      };

   }  // namespace internal

   // template< typename Base >
   // using remove_first_state_b = internal::shuffle_states< internal::remove_first_states< 1 >, Base >;

   // template< template< typename... > class Control, typename Rule >
   // using remove_first_state_r = internal::shuffle_states< internal::remove_first_states< 1 >, Control< Rule > >;

   // template< template< typename... > class Control >
   // struct remove_first_state_n
   // {
   //    template< typename Rule >
   //    using type = internal::shuffle_states< internal::remove_first_states< 1 >, Control< Rule > >;
   // };

   template< std::size_t N, typename Base >
   using remove_first_states_b = internal::shuffle_states< internal::remove_first_states< N >, Base >;

   template< std::size_t N, template< typename... > class Control, typename Rule >
   using remove_first_states_r = internal::shuffle_states< internal::remove_first_states< N >, Control< Rule > >;

   template< std::size_t N, template< typename... > class Control >
   struct remove_first_states_n
   {
      template< typename Rule >
      using type = internal::shuffle_states< internal::remove_first_states< N >, Control< Rule > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
