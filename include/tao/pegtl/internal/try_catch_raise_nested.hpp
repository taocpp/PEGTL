// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TRY_CATCH_RAISE_NESTED_HPP
#define TAO_PEGTL_INTERNAL_TRY_CATCH_RAISE_NESTED_HPP

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/internal/try_catch_raise_nested.hpp"
#else

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "seq.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Exception, typename... Rules >
   struct try_catch_raise_nested
      : try_catch_raise_nested< Exception, seq< Rules... > >
   {};

   template< typename Rule >
   struct try_catch_raise_nested< void, Rule >
   {
      using rule_t = try_catch_raise_nested;
      using subs_t = type_list< Rule >;

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
         const auto p = in.rewind_position();

         try {
            return Control< Rule >::template match< A, M, Action, Control >( in, st... );
         }
         catch( ... ) {
            Control< Rule >::raise_nested( in.previous_position( p ), in, st... );
         }
      }
   };

   template< typename Exception, typename Rule >
   struct try_catch_raise_nested< Exception, Rule >
   {
      using rule_t = try_catch_raise_nested;
      using subs_t = type_list< Rule >;

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
         const auto p = in.rewind_position();

         try {
            return Control< Rule >::template match< A, M, Action, Control >( in, st... );
         }
         catch( const Exception& ) {
            Control< Rule >::raise_nested( in.previous_position( p ), in, st... );
         }
      }
   };

   template< typename Exception >
   struct try_catch_raise_nested< Exception >
      : success
   {};

   template< typename Exception, typename... Rules >
   inline constexpr bool enable_control< try_catch_raise_nested< Exception, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
