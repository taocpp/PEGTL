// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHOOSE_CONTAINER_HPP
#define TAO_PEGTL_INTERNAL_CHOOSE_CONTAINER_HPP

#include <string>
#include <type_traits>
#include <vector>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   struct choose_container
   {
      using type = std::conditional< std::is_same_v< std::decay_t< Data >, char >, std::string, std::vector< Data > >;
   };

   template< typename Data >
   using choose_container_t = typename choose_container< Data >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
