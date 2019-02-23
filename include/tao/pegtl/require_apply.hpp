// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_REQUIRE_APPLY_HPP
#define TAO_PEGTL_REQUIRE_APPLY_HPP

#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "match.hpp"
#include "rewind_mode.hpp"

#include "internal/require_type.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      struct require_apply
      {
         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            internal::require_type< decltype( Control< Rule >::template apply< Action >( std::declval< const typename Input::action_t& >(), in, st... ) ) >();
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
