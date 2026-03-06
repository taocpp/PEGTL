// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_SOURCE_HPP

#include <filesystem>
#include <string>
#include <string_view>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../parse_nested.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "input_with_fakes.hpp"
#include "input_with_funcs.hpp"
#include "input_with_start.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule >
   struct source
   {
      using rule_t = source;
      using subs_t = empty_list;

      using input_t = input_with_fakes< input_with_funcs< input_with_start< view_input< char > > > >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         return match_impl< A, Action, Control >( in, in.direct_source(), st... );
      }

      template< apply_mode A,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match_impl( ParseInput& in, const std::string& source, States&&... st )
      {
         input_t i2( source.data(), source.size() );
         return parse_nested< Rule, Action, Control, A, rewind_mode::optional >( in, i2, st... );
      }

      template< apply_mode A,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match_impl( ParseInput& in, const std::string_view source, States&&... st )
      {
         input_t i2( source.data(), source.size() );
         return parse_nested< Rule, Action, Control, A, rewind_mode::optional >( in, i2, st... );
      }

      template< apply_mode A,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match_impl( ParseInput& in, const std::filesystem::path& source, States&&... st )
      {
         return match_impl< A, Action, Control >( in, source.native(), st... );
      }
   };

   template< typename Rule >
   inline constexpr bool enable_control< source< Rule > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
