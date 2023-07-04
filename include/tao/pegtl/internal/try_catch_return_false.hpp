// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TRY_CATCH_RETURN_FALSE_HPP
#define TAO_PEGTL_INTERNAL_TRY_CATCH_RETURN_FALSE_HPP

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/internal/try_catch_return_false.hpp"
#else

#include <type_traits>

#include "enable_control.hpp"
#include "seq.hpp"
#include "success.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Exception, typename... Rules >
   struct try_catch_return_false
      : try_catch_return_false< Exception, seq< Rules... > >
   {};

   template< typename Exception >
   struct try_catch_return_false< Exception >
      : success
   {};

   template< typename Rule >
   struct try_catch_return_false< void, Rule >
   {
      using rule_t = try_catch_return_false;
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
         auto m = in.template auto_rewind< M >();
         using m_t = decltype( m );

         try {
            return m( Control< Rule >::template match< A, m_t::next_rewind_mode, Action, Control >( in, st... ) );
         }
         catch( ... ) {
            return false;
         }
      }
   };

   template< typename Exception, typename Rule >
   struct try_catch_return_false< Exception, Rule >
   {
      using rule_t = try_catch_return_false;
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
         auto m = in.template auto_rewind< M >();
         using m_t = decltype( m );

         try {
            return m( Control< Rule >::template match< A, m_t::next_rewind_mode, Action, Control >( in, st... ) );
         }
         catch( const Exception& ) {
            return false;
         }
      }
   };

   template< typename Exception, typename... Rules >
   inline constexpr bool enable_control< try_catch_return_false< Exception, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
