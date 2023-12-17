// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_PARSE_HPP
#define TAO_PEGTL_PARSE_HPP

#include <type_traits>

#include "apply_mode.hpp"
#include "config.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse_error.hpp"
#include "rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             apply_mode A = apply_mode::action,
             rewind_mode M = rewind_mode::optional,
             typename ParseInput,
             typename... States >
   auto parse( ParseInput&& in, States&&... st )
   {
      static_assert( !std::is_const_v< ParseInput > );
      return Control< Rule >::template match< A, M, Action, Control >( in, st... );
   }

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             apply_mode A = apply_mode::action,
             rewind_mode M = rewind_mode::optional,
             typename Ambient,
             typename ParseInput,
             typename... States >
   auto parse_nested( const Ambient& am, ParseInput&& in, States&&... st )
   {
      static_assert( !std::is_const_v< ParseInput > );
#if defined( __cpp_exceptions )
      try {
         return parse< Rule, Action, Control, A, M >( in, st... );
      }
      catch( std::exception& /*unused*/ ) {
         Control< Rule >::raise_nested( am, st... );
      }
#else
      (void)am;
      return parse< Rule, Action, Control, A, M >( in, st... );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
