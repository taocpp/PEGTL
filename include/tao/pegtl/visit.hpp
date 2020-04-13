// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_VISIT_HPP
#define TAO_PEGTL_VISIT_HPP

#include <type_traits>

#include "config.hpp"
#include "rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Type, typename... Types >
      inline constexpr bool contains = ( std::is_same_v< Type, Types > || ... );

      template< typename Rules, typename Todo, typename Done >
      struct filter
      {
         using type = Todo;
      };

      template< typename Rule, typename... Rules, typename... Todo, typename... Done >
      struct filter< rule_list< Rule, Rules... >, rule_list< Todo... >, rule_list< Done... > >
         : filter< rule_list< Rules... >, std::conditional_t< contains< Rule, Todo..., Done... >, rule_list< Todo... >, rule_list< Rule, Todo... > >, rule_list< Done... > >
      {};

      template< typename Rules, typename Todo, typename Done >
      using filter_t = typename filter< Rules, Todo, Done >::type;

      template< typename... >
      struct concat
      {
         using type = empty_list;
      };

      template< typename R >
      struct concat< R >
      {
         using type = R;
      };

      template< typename... R0, typename... R1 >
      struct concat< rule_list< R0... >, rule_list< R1... > >
      {
         using type = rule_list< R0..., R1... >;
      };

      template< typename... Ts >
      using concat_t = typename concat< Ts... >::type;

      template< typename... R0, typename... R1, typename T, typename... Ts >
      struct concat< rule_list< R0... >, rule_list< R1... >, T, Ts... >
         : concat< rule_list< R0..., R1... >, concat_t< T, Ts... > >
      {};

      template< template< typename... > class Func, typename Done, typename... Rules >
      struct visitor
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            ( Func< Rules >::visit( args... ), ... );
            using NextDone = concat_t< rule_list< Rules... >, Done >;
            using NextSubs = concat_t< typename Rules::subs_t... >;
            using NextTodo = filter_t< NextSubs, empty_list, NextDone >;
            if constexpr( !std::is_same_v< NextTodo, empty_list > ) {
               visit_next< NextDone >( NextTodo(), args... );
            }
         }

      private:
         template< typename NextDone, typename... NextTodo, typename... Args >
         static void visit_next( rule_list< NextTodo... > /*unused*/, Args&&... args )
         {
            visitor< Func, NextDone, NextTodo... >::visit( args... );
         }
      };

   }  // namespace internal

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit( Args&&... args )
   {
      internal::visitor< Func, empty_list, Rule >::visit( args... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
