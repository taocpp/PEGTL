// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_BUILDERS_HPP
#define TAO_PEGTL_ACTION_BUILDERS_HPP

#include <tuple>

#include "../config.hpp"
#include "../type_list.hpp"

#include "build_for.hpp"
#include "build_to.hpp"
#include "multi_traits.hpp"
#include "repeat_traits.hpp"
#include "require_apply.hpp"
#include "require_apply0.hpp"
#include "value_traits.hpp"

#include "internal/builder_utility.hpp"
#include "internal/deliver_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, auto Value >
   struct case_
   {
      using rule_t = Rule;

      static constexpr auto value = Value;
   };

   template< typename... Cases >
   struct cases  // cases_producer
   {
      static_assert( sizeof...( Cases ) > 0 );

      using rules_t = type_list< typename Cases::rule_t... >;

      template< typename MatchedRule >
      using producer_t = internal::const_producer< internal::case_lookup< MatchedRule, Cases... >::value >;
   };

   template< auto S, auto... Values >
   struct const_to
      : require_apply0
   {
      using target_t = internal::delivery_target_t< S >;
      using result_t = internal::delivery_result_t< S >;

      template< typename... States >
      static void apply0( States&&... st )
      {
         internal::deliver< S >( std::get< target_t& >( std::tie( st... ) ), result_t( Values... ) );
      }
   };

   template< typename Rule, auto... Values >
   using const_for = build_for< Rule, internal::const_producer< Values... > >;

   template< auto S >
   struct value_to
      : require_apply
   {
      using target_t = internal::delivery_target_t< S >;
      using result_t = internal::delivery_result_t< S >;

      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& in, States&&... st )
      {
         internal::deliver< S >( std::get< target_t& >( std::tie( st... ) ), value_traits< result_t >::convert( in ) );
      }
   };

   template< typename Rule >
   using value_for = build_for< Rule, internal::value_producer< value_traits > >;

   template< auto S, typename... Args >
   using multi_to = build_to< S, internal::multi_producer< multi_traits, Args... > >;

   template< typename Rule, typename... Args >
   using multi_for = build_for< Rule, internal::multi_producer< multi_traits, Args... > >;

   template< auto S, typename Arg >
   using optional_to = build_to< S, internal::optional_producer< Arg > >;

   template< typename Rule, typename Arg >
   using optional_for = build_for< Rule, internal::optional_producer< Arg > >;

   template< auto S, typename Arg >
   using repeat_to = build_to< S, internal::repeat_producer< repeat_traits, Arg > >;

   template< typename Rule, typename Arg >
   using repeat_for = build_for< Rule, internal::repeat_producer< repeat_traits, Arg > >;

   template< auto S, typename... Args >
   using variant_to = build_to< S, internal::variant_producer< Args... > >;

   template< typename Rule, typename... Args >
   using variant_for = build_for< Rule, internal::variant_producer< Args... > >;

   template< auto S, typename... Cases >
   using cases_to = multi_to< S, cases< Cases... > >;

   template< typename Rule, typename... Cases >
   using cases_for = multi_for< Rule, cases< Cases... > >;

   template< auto S, typename Object, template< typename... > class ObjectAction >
   using create_to = build_to< S, internal::create_producer< Object, ObjectAction > >;

   template< typename Rule, typename Object, template< typename... > class ObjectAction >
   using create_for = build_for< Rule, internal::create_producer< Object, ObjectAction > >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
