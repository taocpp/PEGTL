// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_SKIP_HPP
#define TAO_PEGTL_CONTRIB_SKIP_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // this is currently experimental and may change at any time
   template< typename How, typename Where, template< typename... > class Base = normal >
   struct skip
   {
      template< typename Rule >
      struct control
         : Base< Rule >
      {
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
            // TODO: if we only skip after but not before the actual rule,
            // we would not need this marker.
            auto m = in.template mark< M >();

            // TODO: different conditions for before/after skipping?
            if( Where::template value< Rule > ) {
               // TODO: assert on result to be successful?
               (void)TAO_PEGTL_NAMESPACE::match< How, apply_mode::nothing, M, nothing, normal >( in );
            }

            const bool result = Base< Rule >::template match< A, M, Action, Control >( in, st... );

            // TODO: different conditions for before/after skipping?
            if( result && Where::template value< Rule > ) {
               // TODO: assert on result to be successful?
               (void)TAO_PEGTL_NAMESPACE::match< How, apply_mode::nothing, M, nothing, normal >( in );
            }

            return m( result );
         }
      };
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
