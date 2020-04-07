// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_SUCCESS_HPP
#define TAO_PEGTL_INTERNAL_SUCCESS_HPP

#include "../config.hpp"

#include "skip_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct success
   {
      using rule_t = success;

      template< typename Input >
      [[nodiscard]] static bool match( Input& /*unused*/ ) noexcept
      {
         return true;
      }
   };

   template<>
   inline constexpr bool skip_control< success > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
