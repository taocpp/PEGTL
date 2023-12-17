// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IS_BUFFER_HPP
#define TAO_PEGTL_INTERNAL_IS_BUFFER_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "has_buffer_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct is_buffer
   {
      using rule_t = is_buffer;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& /*unused*/ ) noexcept
      {
         return has_buffer_size< ParseInput >;
      }
   };

   template<>
   inline constexpr bool enable_control< is_buffer > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
