// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_ANALYZE_TRAITS_IMPL_HPP
#define TAO_PEGTL_DEBUG_ANALYZE_TRAITS_IMPL_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../forward.hpp"

#include "../internal/rules.hpp"

#include "analyze_traits_base.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name, template< typename... > class Action, typename... Rules >
   struct analyze_traits< Name, internal::action< Action, Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename Peek >
   struct analyze_traits< Name, internal::any< Peek > >
      : analyze_any_traits<>
   {};

   template< typename Name, typename... Actions >
   struct analyze_traits< Name, internal::apply< Actions... > >
      : analyze_opt_traits<>
   {};

   template< typename Name, typename... Actions >
   struct analyze_traits< Name, internal::apply0< Actions... > >
      : analyze_opt_traits<>
   {};

   template< typename Name, char... Cs >
   struct analyze_traits< Name, internal::ascii_istring< Cs... > >
      : analyze_any_traits<>
   {
      static_assert( sizeof...( Cs ) > 0 );
   };

   template< typename Name, char... Cs >
   struct analyze_traits< Name, internal::ascii_string< Cs... > >
      : analyze_any_traits<>
   {
      static_assert( sizeof...( Cs ) > 0 );
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::at< Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::bof >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::bol >
      : analyze_opt_traits<>
   {};

   template< typename Name, std::size_t Count, typename Reference >
   struct analyze_traits< Name, internal::consume< Count, Reference > >
      : std::conditional_t< ( Count > 0 ), analyze_any_traits<>, analyze_opt_traits<> >
   {};

   template< typename Name, template< typename... > class Control, typename... Rules >
   struct analyze_traits< Name, internal::control< Control, Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::disable< Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::enable< Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::eof >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::eol >
      : analyze_any_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::eolf >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::everything >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::failure >
      : analyze_any_traits<>
   {};

   // No analyze_traits for internal::function<> for obvious reasons.

   template< typename Name, typename Rule, typename... Actions >
   struct analyze_traits< Name, internal::if_apply< Rule, Actions... > >
      : analyze_traits< Name, typename Rule::rule_t >
   {};

   template< typename Name, typename Cond, typename Then, typename Else >
   struct analyze_traits< Name, internal::if_then_else< Cond, Then, Else > >
      : analyze_traits< Name, typename internal::sor< internal::seq< Cond, Then >, Else >::rule_t >
   {};

   template< typename Name, std::size_t Count, typename Peek >
   struct analyze_traits< Name, internal::many< Count, Peek > >
      : std::conditional_t< ( Count != 0 ), analyze_any_traits<>, analyze_opt_traits<> >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::not_at< Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::opt< Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::partial< Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::plus< Rules... > >
      : analyze_traits< Name, typename internal::seq< Rules..., internal::opt< Name > >::rule_t >
   {};

   template< typename Name, typename Head, typename... Rules >
   struct analyze_traits< Name, internal::rematch< Head, Rules... > >
      : analyze_traits< Name, typename internal::sor< Head, internal::sor< internal::seq< Rules, internal::consume< 1, void > >... > >::rule_t >  // TODO: Correct (enough)?
   {};

   template< typename Name, std::size_t Cnt, typename... Rules >
   struct analyze_traits< Name, internal::rep< Cnt, Rules... > >
      : std::conditional_t< ( Cnt != 0 ), analyze_seq_traits< Rules... >, analyze_opt_traits< Rules... > >
   {};

   template< typename Name, std::size_t Min, std::size_t Max, typename... Rules >
   struct analyze_traits< Name, internal::rep_min_max< Min, Max, Rules... > >
      : std::conditional_t< ( Min != 0 ), analyze_seq_traits< Rules... >, analyze_opt_traits< Rules... > >
   {};

   template< typename Name, std::size_t Max, typename... Rules >
   struct analyze_traits< Name, internal::rep_opt< Max, Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name, typename Rule, typename... Rules >
   struct analyze_traits< Name, internal::seq< Rule, Rules... > >
      : analyze_seq_traits< Rule, Rules... >
   {};

   template< typename Name, typename Rule, typename... Rules >
   struct analyze_traits< Name, internal::sor< Rule, Rules... > >
      : analyze_sor_traits< Rule, Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::star< Rules... > >
      : analyze_opt_traits< Rules..., Name >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::star_partial< Rules... > >
      : analyze_opt_traits< Rules..., Name >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::star_strict< Rules... > >
      : analyze_opt_traits< Rules..., Name >
   {};

   template< typename Name, typename State, typename... Rules >
   struct analyze_traits< Name, internal::state< State, Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::strict< Rules... > >
      : analyze_opt_traits< Rules... >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::success >
      : analyze_opt_traits<>
   {};

   template< typename Name, typename Rule >
   struct analyze_traits< Name, internal::tester< Rule > >
      : analyze_any_traits<>
   {};

   template< typename Name, char32_t... Cs >
   struct analyze_traits< Name, internal::unicode_string< Cs... > >
      : analyze_any_traits<>
   {
      static_assert( sizeof...( Cs ) > 0 );
   };

   template< typename Name, typename Cond >
   struct analyze_traits< Name, internal::until< Cond > >
      : analyze_traits< Name, typename Cond::rule_t >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::until< Cond, Rules... > >
      : analyze_traits< Name, typename internal::seq< internal::star< Rules... >, Cond >::rule_t >
   {};

#if defined( __cpp_exceptions )
   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::if_must< true, Cond, Rules... > >
      : analyze_opt_traits< Cond, Rules... >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::if_must< false, Cond, Rules... > >
      : analyze_seq_traits< Cond, Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::must< Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename T >
   struct analyze_traits< Name, internal::raise< T > >
      : analyze_any_traits<>
   {};

   template< typename Name, typename Exception, typename... Rules >
   struct analyze_traits< Name, internal::try_catch_raise_nested< Exception, Rules... > >
      : analyze_seq_traits< Rules... >
   {};

   template< typename Name, typename Exception, typename... Rules >
   struct analyze_traits< Name, internal::try_catch_return_false< Exception, Rules... > >
      : analyze_seq_traits< Rules... >
   {};
#endif

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
