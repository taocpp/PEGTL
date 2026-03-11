// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CONTAINER_FOR_HPP
#define TAO_PEGTL_INTERNAL_CONTAINER_FOR_HPP

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   struct container_for_data
   {
      using type = std::conditional_t< std::is_same_v< std::decay_t< Data >, char >, std::string, std::vector< Data > >;
   };

   template< typename Data >
   using container_for_data_t = typename container_for_data< Data >::type;

   template< typename Container >
   struct container_for_container
   {
      using type = std::conditional_t< std::is_same_v< std::decay_t< Container >, std::string_view >, std::string, Container >;
   };

   template< typename Container >
   using container_for_container_t = typename container_for_container< Container >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
