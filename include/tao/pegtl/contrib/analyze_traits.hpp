// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ANALYZE_TRAITS_HPP
#define TAO_PEGTL_CONTRIB_ANALYZE_TRAITS_HPP

#include <type_traits>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "forward.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // The reduced rules do not need to be equivalent to the original rule, they only
   // need to be "good enough" for the cycle analysis to find all the problems...

   template< typename Name, template< typename... > class Action, typename... Rules >
   struct analyze_traits< Name, internal::action< Action, Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name, typename Peek >
   struct analyze_traits< Name, internal::any< Peek > >
   {
      using reduced = internal::bytes< 1 >;
   };

   template< typename Name, typename... Actions >
   struct analyze_traits< Name, internal::apply< Actions... > >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name, typename... Actions >
   struct analyze_traits< Name, internal::apply0< Actions... > >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::at< Rules... > >
      : analyze_traits< Name, internal::opt< Rules... > >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::bof >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::bol >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name, unsigned Cnt >
   struct analyze_traits< Name, internal::bytes< Cnt > >
   {
      using reduced = std::conditional_t< bool( Cnt ), internal::bytes< 1 >, internal::opt<> >;
   };

   template< typename Name, template< typename... > class Control, typename... Rules >
   struct analyze_traits< Name, internal::control< Control, Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::disable< Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::discard >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::enable< Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::eof >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::eol >
   {
      using reduced = internal::bytes< 1 >;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::eolf >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::failure >
   {
      using reduced = internal::bytes< 1 >;  // Can never succeed, wherefore "always consumes on success" is trivially true.
   };

   template< typename Name, typename Rule, typename... Actions >
   struct analyze_traits< Name, internal::if_apply< Rule, Actions... > >
      : analyze_traits< Name, typename Rule::rule_t >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::if_must< true, Cond, Rules... > >
      : analyze_traits< Name, internal::opt< Cond, Rules... > >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::if_must< false, Cond, Rules... > >
      : analyze_traits< Name, internal::seq< Cond, Rules... > >
   {};

   template< typename Name, typename Cond, typename Then, typename Else >
   struct analyze_traits< Name, internal::if_then_else< Cond, Then, Else > >
   {
      using reduced = internal::sor< internal::seq< Cond, Then >, Else >;
   };

   template< typename Name, char... Cs >
   struct analyze_traits< Name, internal::istring< Cs... > >
   {
      using reduced = std::conditional_t< bool( sizeof...( Cs ) ), internal::bytes< 1 >, internal::opt<> >;
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::must< Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::not_at< Rules... > >
      : analyze_traits< Name, internal::opt< Rules... > >
   {};

   template< typename Name, internal::result_on_found R, typename Peek, typename Peek::data_t... Cs >
   struct analyze_traits< Name, internal::one< R, Peek, Cs... > >
   {
      using reduced = internal::bytes< 1 >;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::opt<> >
      : analyze_traits< Name, internal::success >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::opt< Rules... > >
   {
      using reduced = internal::opt< Rules... >;
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::plus< Rules... > >
   {
      using reduced = internal::seq< Rules..., internal::opt< Name > >;
   };

   template< typename Name, typename T >
   struct analyze_traits< Name, internal::raise< T > >
   {
      using reduced = internal::bytes< 1 >;  // Can never succeed, wherefore "always consumes on success" is trivially true.
   };

   template< typename Name, internal::result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct analyze_traits< Name, internal::range< R, Peek, Lo, Hi > >
   {
      using reduced = internal::bytes< 1 >;
   };

   template< typename Name, typename Peek, typename Peek::data_t... Cs >
   struct analyze_traits< Name, internal::ranges< Peek, Cs... > >
   {
      using reduced = internal::bytes< 1 >;
   };

   template< typename Name, typename Head, typename... Rules >
   struct analyze_traits< Name, internal::rematch< Head, Rules... > >
   {
      using reduced = typename analyze_traits< Name, typename Head::rule_t >::reduced;  // TODO: What with Rules?
   };

   template< typename Name, unsigned Cnt, typename... Rules >
   struct analyze_traits< Name, internal::rep< Cnt, Rules... > >
      : analyze_traits< Name, std::conditional_t< bool( Cnt ), internal::seq< Rules... >, internal::opt< Rules... > > >
   {};

   template< typename Name, unsigned Min, unsigned Max, typename... Rules >
   struct analyze_traits< Name, internal::rep_min_max< Min, Max, Rules... > >
      : analyze_traits< Name, std::conditional_t< bool( Min ), internal::seq< Rules... >, internal::opt< Rules... > > >
   {};

   template< typename Name, unsigned Max, typename... Rules >
   struct analyze_traits< Name, internal::rep_opt< Max, Rules... > >
      : analyze_traits< Name, internal::opt< Rules... > >
   {};

   template< typename Name, unsigned Amount >
   struct analyze_traits< Name, internal::require< Amount > >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::seq<> >
      : analyze_traits< Name, internal::success >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::seq< Rules... > >
   {
      using reduced = internal::seq< Rules... >;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::sor<> >
      : analyze_traits< Name, internal::failure >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::sor< Rules... > >
   {
      using reduced = internal::sor< Rules... >;
   };

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::star< Rules... > >
      : analyze_traits< Name, internal::opt< Rules..., Name > >
   {};

   template< typename Name, typename State, typename... Rules >
   struct analyze_traits< Name, internal::state< State, Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name, char... Cs >
   struct analyze_traits< Name, internal::string< Cs... > >
   {
      using reduced = std::conditional_t< bool( sizeof...( Cs ) ), internal::bytes< 1 >, opt<> >;
   };

   template< typename Name >
   struct analyze_traits< Name, internal::success >
   {
      using reduced = internal::opt<>;
   };

   template< typename Name, typename Exception, typename... Rules >
   struct analyze_traits< Name, internal::try_catch_type< Exception, Rules... > >
      : analyze_traits< Name, internal::seq< Rules... > >
   {};

   template< typename Name, typename Cond >
   struct analyze_traits< Name, internal::until< Cond > >
      : analyze_traits< Name, typename Cond::rule_t >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct analyze_traits< Name, internal::until< Cond, Rules... > >
      : analyze_traits< Name, internal::seq< internal::star< Rules... >, Cond > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
