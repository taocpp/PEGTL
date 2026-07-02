// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_CHARCONV_TO_HPP
#define TAO_PEGTL_EXTRA_CHARCONV_TO_HPP

#include <cstdint>
#include <tuple>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "overflow_mode.hpp"

#include "../action/internal/apply_to_call.hpp"
#include "../action/internal/apply_to_traits.hpp"
#include "../action/internal/integer_utility.hpp"

#include "internal/is_from_chars.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< auto S >
   struct from_chars_to
      : maybe_nothing
   {
      using target_t = internal::apply_to_target_t< S >;
      using object_t = internal::apply_to_object_t< S >;

      static_assert( internal::is_proper_integer_v< object_t > );

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         static_assert( internal::is_from_chars_v< object_t, typename Rule::rule_t > );

         if constexpr( A == apply_mode::enabled ) {
            object_t object;

            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, object, st... ) ) {
               target_t& out = std::get< target_t& >( std::tie( st... ) );
               internal::apply_to_call< S >::call( out, object );
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
