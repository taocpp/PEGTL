// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CONSUME_HPP
#define TAO_PEGTL_INTERNAL_CONSUME_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::size_t Count >
   struct consume
   {
      using rule_t = consume;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept
      {
         if( in.size( Count ) >= Count ) {
            in.template consume< eol_unknown_tag >( Count );
            return true;
         }
         return false;
      }
   };

   template<>
   struct consume< 0 >
      : success
   {};

   template< std::size_t Count >
   inline constexpr bool enable_control< consume< Count > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
