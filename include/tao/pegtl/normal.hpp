// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_NORMAL_HPP
#define TAO_PEGTL_NORMAL_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "match.hpp"
#include "rewind_mode.hpp"

#include "internal/action_input.hpp"
#include "internal/enable_control.hpp"
#include "internal/has_match.hpp"
#include "internal/type_traits.hpp"

#if defined( __cpp_exceptions )
#include "demangle.hpp"
#include "parse_error.hpp"
#else
#include <exception>

#include "internal/dependent_false.hpp"
#endif

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct normal
   {
      static constexpr bool enable = internal::enable_control< Rule >;

      template< typename ParseInput, typename... States >
      static void start( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< typename ParseInput, typename... States >
      static void success( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< typename ParseInput, typename... States >
      static void failure( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< typename ParseInput, typename... States >
      [[noreturn]] static void raise( const ParseInput& in, States&&... /*unused*/ )
      {
#if defined( __cpp_exceptions )
         if constexpr( internal::has_error_message< Rule > ) {
            throw_parse_error( Rule::error_message, in );
         }
         else {
            throw_parse_error( "parse error matching " + std::string( demangle< Rule >() ), in );
         }
#else
         static_assert( internal::dependent_false< Rule >, "Exception support required for normal< Rule >::raise()!" );
         (void)in;
         std::terminate();
#endif
      }

      template< typename Ambient, typename ParseInput, typename... States >
      [[noreturn]] static void raise_nested( const Ambient& am, const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
#if defined( __cpp_exceptions )
         if constexpr( internal::has_error_message< Rule > ) {
            throw_with_nested_parse_error( Rule::error_message, am );
         }
         else {
            throw_with_nested_parse_error( "parse error matching " + std::string( demangle< Rule >() ), am );
         }
#else
         static_assert( internal::dependent_false< Rule >, "Exception support required for normal< Rule >::raise_nested()!" );
         (void)am;
         std::terminate();
#endif
      }

      template< template< typename... > class Action,
                typename RewindPosition,
                typename ParseInput,
                typename... States >
      static auto apply( const RewindPosition& begin, const ParseInput& in, States&&... st ) noexcept( noexcept( Action< Rule >::apply( std::declval< const internal::action_input< ParseInput >& >(), st... ) ) )
         -> decltype( Action< Rule >::apply( std::declval< const internal::action_input< ParseInput >& >(), st... ) )
      {
         const internal::action_input< ParseInput > ai( begin, in );
         return Action< Rule >::apply( ai, st... );
      }

      template< template< typename... > class Action,
                typename ParseInput,
                typename... States >
      static auto apply0( const ParseInput& /*unused*/, States&&... st ) noexcept( noexcept( Action< Rule >::apply0( st... ) ) )
         -> decltype( Action< Rule >::apply0( st... ) )
      {
         return Action< Rule >::apply0( st... );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( internal::has_match< bool, Rule, A, M, Action, Control, ParseInput, States... > ) {
            return Action< Rule >::template match< Rule, A, M, Action, Control >( in, st... );
         }
         else {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
