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

      template< template< typename... > class Func, typename Todo, typename Done >
      struct visitor
      {
         template< typename... Args >
         static void visit( Args&&... /*unused*/ )
         {}
      };

      template< template< typename... > class Func, typename Rule, typename... Todo, typename... Done >
      struct visitor< Func, rule_list< Rule, Todo... >, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            visit_impl( typename Rule::subs_t(), args... );
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... > /*unused*/, Args&&... args )
         {
            Func< Rule, Subs... >::visit( args... );
            using NextDone = rule_list< Rule, Done... >;
            using NextTodo = typename filter< rule_list< Subs... >, rule_list< Todo... >, NextDone >::type;
            visitor< Func, NextTodo, NextDone >::visit( args... );
         }
      };

   }  // namespace internal

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit( Args&&... args )
   {
      internal::visitor< Func, rule_list< Rule >, rule_list<> >::visit( args... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
