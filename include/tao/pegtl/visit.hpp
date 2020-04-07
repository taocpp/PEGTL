// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_VISIT_HPP
#define TAO_PEGTL_VISIT_HPP

#include <type_traits>
#include <utility>

#include "config.hpp"
#include "rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Type, typename... Types >
      inline constexpr bool contains = ( std::is_same_v< Type, Types > || ... );

      template< template< typename... > class Func, typename Todo, typename Done, typename = void >
      struct visitor;

      template< template< typename... > class Func, typename... Done >
      struct visitor< Func, rule_list<>, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... /*unused*/ )
         {}
      };

      template< template< typename... > class Func, typename Rule, typename... Todo, typename... Done >
      struct visitor< Func, rule_list< Rule, Todo... >, rule_list< Done... >, std::enable_if_t< contains< Rule, Done... > > >
         : visitor< Func, rule_list< Todo... >, rule_list< Done... > >
      {};

      template< template< typename... > class Func, typename Rule, typename... Todo, typename... Done >
      struct visitor< Func, rule_list< Rule, Todo... >, rule_list< Done... >, std::enable_if_t< !contains< Rule, Done... > > >
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            visit_impl( typename Rule::subs_t(), std::forward< Args >( args )... );
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... > /*unused*/, Args&&... args )
         {
            Func< Rule, Subs... >::call( std::forward< Args >( args )... );
            visitor< Func, rule_list< Subs..., Todo... >, rule_list< Rule, Done... > >::visit( std::forward< Args >( args )... );
         }
      };

   }  // namespace internal

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit( Args&&... args )
   {
      internal::visitor< Func, rule_list< Rule >, rule_list<> >::visit( std::forward< Args >( args )... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
