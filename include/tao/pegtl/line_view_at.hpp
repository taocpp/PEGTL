// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_LINE_VIEW_AT_HPP
#define TAO_PEGTL_LINE_VIEW_AT_HPP

#include <string_view>
#include <type_traits>

#include "apply_mode.hpp"
#include "config.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "rewind_mode.hpp"

#include "internal/argv_input.hpp"
#include "internal/at.hpp"
#include "internal/eolf.hpp"
#include "internal/input_traits.hpp"
#include "internal/scan_input.hpp"
#include "internal/until.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // The assumption is that argv consists of strings without eols.

   template< typename Input, typename Position >
   [[nodiscard]] auto begin_of_line( const Input& in, const Position& /*unused*/ ) noexcept -> std::enable_if_t< std::is_base_of_v< internal::argv_input, Input >, const char* >
   {
      return in.start();
   }

   template< typename Input, typename Position >
   [[nodiscard]] auto begin_of_line( const Input& in, const Position& p ) noexcept -> std::enable_if_t< !std::is_base_of_v< internal::argv_input, Input >, decltype( in.current() ) >
   {
      return in.previous( p ) - ( p.column - 1 );
   }

   template< typename Input, typename Position >
   [[nodiscard]] auto end_of_line_or_file( const Input& in, const Position& /*unused*/ ) noexcept -> std::enable_if_t< std::is_base_of_v< internal::argv_input, Input >, const char* >
   {
      return in.end();
   }

   template< typename Input, typename Position >
   [[nodiscard]] auto end_of_line_or_file( const Input& in, const Position& p ) noexcept -> std::enable_if_t< !std::is_base_of_v< internal::argv_input, Input >, decltype( in.current() ) >
   {
      using grammar = internal::until< internal::at< internal::eolf< typename Input::eol_rule > > >;
      internal::scan_input< typename Input::data_t > i2( in.previous( p ), in.end() );  // TODO: Start before in.at( p ) to correctly handle the middle of a multi-token EOL?
      (void)normal< grammar >::template match< apply_mode::nothing, rewind_mode::optional, nothing, normal >( i2 );
      return i2.current();
   }

   template< typename Input, typename Position >
   [[nodiscard]] std::string_view line_view_at( const Input& in, const Position& p ) noexcept
   {
      const char* b = begin_of_line( in, p );
      return { b, static_cast< std::size_t >( end_of_line_or_file( in, p ) - b ) };
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
