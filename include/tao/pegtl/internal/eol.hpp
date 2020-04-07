// Copyright (c) 2016-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_EOL_HPP
#define TAO_PEGTL_INTERNAL_EOL_HPP

#include "../config.hpp"

#include "skip_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct eol
   {
      using rule_t = eol;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( Input::eol_t::match( in ) ) )
      {
         return Input::eol_t::match( in ).first;
      }
   };

   template<>
   inline constexpr bool skip_control< eol > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
