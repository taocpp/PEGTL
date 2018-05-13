// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_IF_MUST_HPP
#define TAO_PEGTL_INTERNAL_IF_MUST_HPP

#include "../config.hpp"

#include "must.hpp"
#include "rule_conjunction.hpp"
#include "skip_control.hpp"
#include "trivial.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename... Rules >
         struct if_must;

         template<>
         struct if_must<>
            : trivial< true >
         {
         };

         template< typename Cond >
         struct if_must< Cond >
         {
            using analyze_t = typename Cond::analyze_t;

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               return Control< Cond >::template match< A, M, Action, Control >( in, st... );
            }
         };

         template< typename Cond, typename... Rules >
         struct if_must< Cond, Rules... >
         {
            using analyze_t = analysis::generic< analysis::rule_type::SEQ, Cond, must< Rules... > >;

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               if( Control< Cond >::template match< A, M, Action, Control >( in, st... ) ) {
                  return rule_conjunction< must< Rules >... >::template match< A, M, Action, Control >( in, st... );
               }
               return false;
            }
         };

         template< typename... Rules >
         struct skip_control< if_must< Rules... > > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
