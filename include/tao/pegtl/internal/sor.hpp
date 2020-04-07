// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_SOR_HPP
#define TAO_PEGTL_INTERNAL_SOR_HPP

#include <utility>

#include "../config.hpp"

#include "failure.hpp"
#include "skip_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct sor;

   template< typename... Rules >
   struct sor_impl;

   template<>
   struct sor<>
      : failure
   {
      using rule_t = sor;
   };

   template< typename... Rules >
   struct sor
      : sor_impl< std::index_sequence_for< Rules... >, Rules... >
   {
      using rule_t = sor;
   };

   template< std::size_t... Indices, typename... Rules >
   struct sor_impl< std::index_sequence< Indices... >, Rules... >
   {
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
         return ( Control< Rules >::template match< A, ( ( Indices == ( sizeof...( Rules ) - 1 ) ) ? M : rewind_mode::required ), Action, Control >( in, st... ) || ... );
      }
   };

   template< typename... Rules >
   inline constexpr bool skip_control< sor< Rules... > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
