// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_REP_HPP
#define TAO_PEGTL_INTERNAL_REP_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "skip_control.hpp"
#include "success.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Cnt, typename... Rules >
   struct rep
      : rep< Cnt, seq< Rules... > >
   {
      using rule_t = rep;
      using subs_t = rule_list< Rules... >;
   };

   template< unsigned Cnt >
   struct rep< Cnt >
      : success
   {
      using rule_t = rep;
      using subs_t = empty_list;
   };

   template< typename Rule >
   struct rep< 0, Rule >
      : success
   {
      using rule_t = rep;
      using subs_t = empty_list;  // NOTE: This is a bit strange, but with Cnt == 0 the sub rules are never hit.
   };

   template< unsigned Cnt, typename Rule >
   struct rep< Cnt, Rule >
   {
      using rule_t = rep;
      using subs_t = rule_list< Rule >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         auto m = in.template mark< M >();
         using m_t = decltype( m );

         for( unsigned i = 0; i != Cnt; ++i ) {
            if( !Control< Rule >::template match< A, m_t::next_rewind_mode, Action, Control >( in, st... ) ) {
               return false;
            }
         }
         return m( true );
      }
   };

   template< unsigned Cnt, typename... Rules >
   inline constexpr bool skip_control< rep< Cnt, Rules... > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
