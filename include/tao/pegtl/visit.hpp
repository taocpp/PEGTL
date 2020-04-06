// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_VISIT_HPP
#define TAO_PEGTL_VISIT_HPP

#include <type_traits>

#include "config.hpp"
#include "traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename... Types > struct contains;

      template<>
      struct contains<>
      {
         template< typename > constexpr static bool value = false;
      };

      template< typename Type, typename... Types >
      struct contains< Type, Types... >
      {
         template< typename U > constexpr static bool value = std::is_same_v< Type, U > || contains< Types... >::template value< U >;
      };

      template< template< typename... > class Func, typename Todo, typename Done >
      struct visitor;

      template< template< typename... > class Func, typename... Done >
      struct visitor< Func, rule_list<>, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... /*unused*/ )
         {
         }
      };

      template< template< typename... > class Func, typename Rule, typename... Todo, typename... Done >
      struct visitor< Func, rule_list< Rule, Todo... >, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            if constexpr( !( contains< Done... >::template value< Rule > ) ) {
               Func< Rule >::call( args... );
               visit_impl( typename traits< typename Rule::rule_t >::subs(), args... );
            }
            else {
               visitor< Func, rule_list< Todo... >, rule_list< Done... > >::visit( args... );
            }
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... >&& /*unused*/, Args&&... args )
         {
            visitor< Func, rule_list< Subs..., Todo... >, rule_list< Rule, Done... > >::visit( args... );
         }
      };

      template< template< typename... > class Func, typename Todo, typename Done >
      struct visitor_with_subs;

      template< template< typename... > class Func, typename... Done >
      struct visitor_with_subs< Func, rule_list<>, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... /*unused*/ )
         {
         }
      };

      template< template< typename... > class Func, typename Rule, typename... Todo, typename... Done >
      struct visitor_with_subs< Func, rule_list< Rule, Todo... >, rule_list< Done... > >
      {
         template< typename... Args >
         static void visit( Args&&... args )
         {
            visit_impl( typename traits< typename Rule::rule_t >::subs(), args... );
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... >&& /*unused*/, Args&&... args )
         {
            if constexpr( !( contains< Done... >::template value< Rule > ) ) {
               Func< Rule, Subs... >::call( args... );
               visitor_with_subs< Func, rule_list< Subs..., Todo... >, rule_list< Rule, Done... > >::visit( args... );
            }
            else {
               visitor_with_subs< Func, rule_list< Todo... >, rule_list< Done... > >::visit( args... );
            }
         }
      };

   }  // namespace internal

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit( Args&&... args )
   {
      internal::visitor< Func, rule_list< Rule >, rule_list<> >::visit( args... );
   }

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit_with_subs( Args&&... args )
   {
      internal::visitor_with_subs< Func, rule_list< Rule >, rule_list<> >::visit( args... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
