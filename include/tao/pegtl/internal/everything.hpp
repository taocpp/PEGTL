// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_EVERYTHING_HPP
#define TAO_PEGTL_INTERNAL_EVERYTHING_HPP

#include <cstddef>

#include "enable_control.hpp"

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct everything
   {
      using rule_t = everything;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         in.template consume< everything >( in.size( std::size_t( -1 ) ) );
         return true;
      }
   };

   template<>
   inline constexpr bool enable_control< everything > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
