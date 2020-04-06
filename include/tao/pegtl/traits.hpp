// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_TRAITS_HPP
#define TAO_PEGTL_TRAITS_HPP

#include "config.hpp"
#include "rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T, typename = void >
   struct traits
   {};

   template< typename... Rules >
   struct rule_list
   {
      static constexpr unsigned size = sizeof...( Rules );
   };

   using empty_list = rule_list<>;

   template< template< typename... > class Action, typename... Rules >
   struct traits< internal::action< Action, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename Peek >
   struct traits< internal::any< Peek > >
   {
      using subs = empty_list;
   };

   template< typename... Actions >
   struct traits< internal::apply< Actions... > >
   {
      using subs = empty_list;
   };

   template< typename... Actions >
   struct traits< internal::apply0< Actions... > >
   {
      using subs = empty_list;
   };

   template< typename... Rules >
   struct traits< internal::at< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template<>
   struct traits< internal::bof >
   {
      using subs = empty_list;
   };

   template<>
   struct traits< internal::bol >
   {
      using subs = empty_list;
   };

   template< unsigned Cnt >
   struct traits< internal::bytes< Cnt > >
   {
      using subs = empty_list;
   };

   template< template< typename... > class Control, typename... Rules >
   struct traits< internal::control< Control, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename... Rules >
   struct traits< internal::disable< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template<>
   struct traits< internal::discard >
   {
      using subs = empty_list;
   };

   template< typename... Rules >
   struct traits< internal::enable< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template<>
   struct traits< internal::eof >
   {
      using subs = empty_list;
   };

   template<>
   struct traits< internal::eol >
   {
      using subs = empty_list;
   };

   template<>
   struct traits< internal::eolf >
   {
      using subs = empty_list;
   };

   template<>
   struct traits< internal::failure >
   {
      using subs = empty_list;
   };

   template< typename Rule, typename... Actions >
   struct traits< internal::if_apply< Rule, Actions... > >
   {
      using subs = rule_list< Rule >;
   };

   template< bool Default, typename Cond, typename... Rules >
   struct traits< internal::if_must< Default, Cond, Rules... > >
   {
      using subs = rule_list< Cond, Rules... >;
   };

   template< typename Cond, typename Then, typename Else >
   struct traits< internal::if_then_else< Cond, Then, Else > >
   {
      using subs = rule_list< Cond, Then, Else >;
   };

   template< char... Cs >
   struct traits< internal::istring< Cs... > >
   {
      using subs = empty_list;
   };

   template< typename... Rules >
   struct traits< internal::must< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename... Rules >
   struct traits< internal::not_at< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< internal::result_on_found R, typename Peek, typename Peek::data_t... Cs >
   struct traits< internal::one< R, Peek, Cs... > >
   {
      using subs = empty_list;
   };

   template< typename... Rules >
   struct traits< internal::opt< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename... Rules >
   struct traits< internal::plus< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename T >
   struct traits< internal::raise< T > >
   {
      using subs = empty_list;
   };

   template< internal::result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct traits< internal::range< R, Peek, Lo, Hi > >
   {
      using subs = empty_list;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct traits< internal::ranges< Peek, Cs... > >
   {
      using subs = empty_list;
   };

   template< typename Head, typename... Rules >
   struct traits< internal::rematch< Head, Rules... > >
   {
      using subs = rule_list< Head, Rules... >;
   };

   template< unsigned Cnt, typename... Rules >
   struct traits< internal::rep< Cnt, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< unsigned Min, unsigned Max, typename... Rules >
   struct traits< internal::rep_min_max< Min, Max, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< unsigned Max, typename... Rules >
   struct traits< internal::rep_opt< Max, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< unsigned Amount >
   struct traits< internal::require< Amount > >
   {
      using subs = empty_list;
   };

   template< typename... Rules >
   struct traits< internal::seq< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename... Rules >
   struct traits< internal::sor< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename... Rules >
   struct traits< internal::star< Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename State, typename... Rules >
   struct traits< internal::state< State, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< char... Cs >
   struct traits< internal::string< Cs... > >
   {
      using subs = empty_list;
   };

   template<>
   struct traits< internal::success >
   {
      using subs = empty_list;
   };

   template< typename Exception, typename... Rules >
   struct traits< internal::try_catch_type< Exception, Rules... > >
   {
      using subs = rule_list< Rules... >;
   };

   template< typename Cond, typename... Rules >
   struct traits< internal::until< Cond, Rules... > >
   {
      using subs = rule_list< Cond, Rules... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
