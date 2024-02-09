// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INTEGER_SIZE_HPP
#define TAO_PEGTL_INTERNAL_INTEGER_SIZE_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "dependent_false.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Type, typename Data >
   [[nodiscard]] constexpr std::size_t integer_size() noexcept
   {
      static_assert( is_simple_type_v< Type > );
      static_assert( is_simple_type_v< Data > );

      if constexpr( sizeof( Type ) == sizeof( Data ) ) {
         return 1;
      }
      else if constexpr( sizeof( Data ) == 1 ) {
         return sizeof( Type );
      }
      else {
         static_assert( dependent_false< Data > );
      }
   }

   template< typename Type, typename Input>
   [[nodiscard]] constexpr std::size_t integer_input_size() noexcept
   {
      static_assert( std::is_same_v< typename Input::data_t, std::decay_t< decltype( *( std::declval< const Input& >().current() ) ) > > );

      return integer_size< Type, typename Input::data_t >();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
