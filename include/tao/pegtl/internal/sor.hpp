// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_SOR_HPP
#define TAO_PEGTL_INTERNAL_SOR_HPP

#include <utility>

#include "../config.hpp"

#include "enable_control.hpp"
#include "failure.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct sor;

   template<>
   struct sor<>
      : failure
   {};

   template< typename... Rules >
   struct sor
   {
      using rule_t = sor;
      using subs_t = rule_list< Rules... >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                std::size_t... Indices,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( std::index_sequence< Indices... > /*unused*/, Input& in, States&&... st )
      {
         return ( Control< Rules >::template match< A, ( ( Indices == ( sizeof...( Rules ) - 1 ) ) ? M : rewind_mode::required ), Action, Control >( in, st... ) || ... );
      }

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
         return match< A, M, Action, Control >( std::index_sequence_for< Rules... >(), in, st... );
      }
   };

   template< typename... Rules >
   inline constexpr bool enable_control< sor< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
