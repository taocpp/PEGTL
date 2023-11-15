// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_LINE_VIEW_AT_HPP
#define TAO_PEGTL_LINE_VIEW_AT_HPP

#include <string_view>

#include "apply_mode.hpp"
#include "config.hpp"
#include "memory_input.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "rewind_mode.hpp"
#include "tracking_mode.hpp"

#include "internal/at.hpp"
#include "internal/eolf.hpp"
#include "internal/until.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Input >
   [[nodiscard]] const char* begin_of_line( const Input& in, const TAO_PEGTL_NAMESPACE::position& p ) noexcept
   {
      return in.at( p ) - ( p.column - 1 );
   }

   template< typename Input >
   [[nodiscard]] const char* end_of_line_or_file( const Input& in, const TAO_PEGTL_NAMESPACE::position& p ) noexcept
   {
      using input_t = memory_input< tracking_mode::lazy, typename Input::eol_t, const char* >;
      input_t i2( in.at( p ), in.end(), "" );  // TODO: Start before in.at( p ) to correctly handle the middle of a multi-token EOL.
      using grammar = internal::until< internal::at< internal::eolf > >;
      (void)normal< grammar >::match< apply_mode::nothing, rewind_mode::optional, nothing, normal >( i2 );
      return i2.current();
   }

   template< typename Input >
   [[nodiscard]] std::string_view line_view_at( const Input& in, const TAO_PEGTL_NAMESPACE::position& p ) noexcept
   {
      const char* b = begin_of_line( in, p );
      return { b, static_cast< std::size_t >( end_of_line_or_file( in, p ) - b ) };
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
