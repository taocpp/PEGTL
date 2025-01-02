// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NOT_AT_HPP
#define TAO_PEGTL_INTERNAL_NOT_AT_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "failure.hpp"
#include "seq.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct not_at
      : not_at< seq< Rules... > >
   {};

   template< typename Rule >
   struct not_at< Rule >
   {
      using rule_t = not_at;
      using subs_t = type_list< Rule >;

      template< apply_mode,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         const auto m = Control< not_at >::template guard< apply_mode::nothing, rewind_mode::required, Action, Control >( in, st... );
         return !Control< Rule >::template match< apply_mode::nothing, rewind_mode::optional, Action, Control >( in, st... );
      }
   };

   template<>
   struct not_at<>
      : failure
   {};

   template< typename... Rules >
   inline constexpr bool enable_control< not_at< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
