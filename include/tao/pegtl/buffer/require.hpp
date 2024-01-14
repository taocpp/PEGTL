// Copyright (c) 2016-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_REQUIRE_HPP
#define TAO_PEGTL_BUFFER_REQUIRE_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "../internal/success.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< std::size_t Amount >
   struct require;

   template<>
   struct require< 0 >
      : internal::success
   {};

   template< std::size_t Amount >
   struct require
   {
      using rule_t = require;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         return in.size( Amount ) >= Amount;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
