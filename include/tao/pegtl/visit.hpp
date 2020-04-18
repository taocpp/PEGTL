// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_VISIT_HPP
#define TAO_PEGTL_VISIT_HPP

#include <type_traits>

#include "config.hpp"
#include "type_list.hpp"

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
      struct filter< type_list< Rule, Rules... >, type_list< Todo... >, type_list< Done... > >
         : filter< type_list< Rules... >, std::conditional_t< contains< Rule, Todo..., Done... >, type_list< Todo... >, type_list< Rule, Todo... > >, type_list< Done... > >
      {};

      template< typename Rules, typename Todo, typename Done >
      using filter_t = typename filter< Rules, Todo, Done >::type;

      template< template< typename... > class Func, typename Done, typename... Rules >
      struct visitor
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            ( Func< Rules >::visit( args... ), ... );
            using NextDone = type_list_concat_t< type_list< Rules... >, Done >;
            using NextSubs = type_list_concat_t< typename Rules::subs_t... >;
            using NextTodo = filter_t< NextSubs, empty_list, NextDone >;
            if constexpr( !std::is_same_v< NextTodo, empty_list > ) {
               visit_next< NextDone >( NextTodo(), args... );
            }
         }

      private:
         template< typename NextDone, typename... NextTodo, typename... Args >
         static void visit_next( type_list< NextTodo... > /*unused*/, Args&&... args )
         {
            visitor< Func, NextDone, NextTodo... >::visit( args... );
         }
      };

   }  // namespace internal

   template< typename Rule, template< typename... > class Func, typename... Args >
   void visit( Args&&... args )
   {
      internal::visitor< Func, empty_list, Rule >::visit( args... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
