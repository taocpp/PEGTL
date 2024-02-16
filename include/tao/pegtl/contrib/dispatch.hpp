// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_DISPATCH_HPP
#define TAO_PEGTL_CONTRIB_DISPATCH_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../match.hpp"
#include "../rewind_mode.hpp"

#include "../internal/dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename, typename >
      struct tuple_to_action;

      template< typename Rule >
      struct tuple_to_action< Rule, std::tuple<> >
      {
         using type = nothing< Rule >;
      };

      template< typename Rule, template< typename, typename... > class Clause, typename Action, typename... Rules >
      struct tuple_to_action< Rule, std::tuple< Clause< Action, Rules... > > >
      {
         using type = Action;
      };

      template< typename Rule, template< template< typename... > class, typename... > class Clause, template< typename... > class Action, typename... Rules >
      struct tuple_to_action< Rule, std::tuple< Clause< Action, Rules... > > >
      {
         using type = Action< Rule >;
      };

      template< typename Rule, typename... Clauses >
      struct tuple_to_action< Rule, std::tuple< Clauses... > >
      {
         static_assert( dependent_false< Rule, Clauses... >, "more than one action clause applies" );  // TODO: Should we error out or apply all?
      };

      template< typename Rule, typename Clause >
      using clause_to_tuple_t = std::conditional_t< Clause::template enable< Rule >, std::tuple< Clause >, std::tuple<> >;

      template< typename Rule, typename... Clauses >
      using clauses_to_tuple_t = decltype( std::tuple_cat( std::declval< internal::clause_to_tuple_t< Rule, Clauses > >()... ) );

      template< typename Rule, typename... Clauses >
      using clauses_to_action_t = typename tuple_to_action< Rule, clauses_to_tuple_t< Rule, Clauses... > >::type;

   }  // namespace internal

   template< typename Action, typename... Rules >
   struct clause1
   {
      template< typename Rule >
      static constexpr bool enable = ( std::is_same_v< Rule, Rules > || ... );
   };

   template< template< typename... > class Action, typename... Rules >
   struct clause2
   {
      template< typename Rule >
      static constexpr bool enable = ( std::is_same_v< Rule, Rules > || ... );
   };

   template< typename... Clauses >
   struct dispatch
   {
      template< typename Rule >
      struct actions
         : internal::clauses_to_action_t< Rule, Clauses... >
      {};

      template< typename Rule,
                apply_mode A = apply_mode::action,
                rewind_mode M = rewind_mode::optional,
                template< typename... >
                class Control = normal,
                typename ParseInput,
                typename... States >
      static bool parse( ParseInput&& in, States&&... st )
      {
         return TAO_PEGTL_NAMESPACE::match< Rule, A, M, actions, Control >( in, st... );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
