// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INCLUDE_TAO_EXTRA_UNESCAPE_TO_HPP
#define TAO_PEGTL_INCLUDE_TAO_EXTRA_UNESCAPE_TO_HPP

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "../action/internal/apply_to_call.hpp"
#include "../action/internal/apply_to_traits.hpp"

#include "unescape.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< auto S, template< typename... > class Unescape = unescape >
   struct unescape_to
      : maybe_nothing
   {
      // static_assert( std::is_same_v< internal::apply_to_object_t< S >, std::string > );

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( A == apply_mode::enabled ) {
            std::string unescaped;

            if( Control< Rule >::template match< A, M, Unescape, Control >( in, unescaped, st... ) ) {
               using target_t = internal::apply_to_target_t< S >;
               target_t& out = std::get< target_t& >( std::tie( st... ) );
               internal::apply_to_call< S >::call( out, std::move( unescaped ) );
               return true;
            }
            return false;
         }
         else {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
