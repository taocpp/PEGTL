// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_BUILD_TO_HPP
#define TAO_PEGTL_ACTION_BUILD_TO_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "internal/builder_utility.hpp"
#include "internal/deliver_utility.hpp"
#include "internal/single_rule_action.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< auto S, typename Producer >
   struct build_to
      : maybe_nothing
   {
      using target_t = internal::delivery_target_t< S >;
      using result_t = internal::delivery_result_t< S >;
      using output_t = internal::value_slot< result_t >;

      using selected_action = typename Producer::template action< output_t >;

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
            output_t output;

            if( Control< Rule >::template match< A, M, internal::single_rule_action< Rule, selected_action >::template type, Control >( in, output, st... ) ) {
               internal::deliver< S >( std::get< target_t& >( std::tie( st... ) ), output.get() );
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
