// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_VISIT_RT_HPP
#define TAO_PEGTL_VISIT_RT_HPP

#include <set>
#include <typeindex>
#include <type_traits>

#include "config.hpp"
#include "traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< template< typename... > class Traits, template< typename... > class Func, typename Rule >
      struct visitor_rt
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            if( done.emplace( std::type_index( typeid( Rule ) ) ).second ) {
               Func< Rule >::call( args... );
               visitor_rt< Traits, Func, typename Traits< typename Rule::rule_t >::subs >::visit( done, args... );
            }
         }
      };

      template< template< typename... > class Traits, template< typename... > class Func >
      struct visitor_rt< Traits, Func, empty_list >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& /*unused*/, Args&&... /*unused*/ )
         {
         }
      };

      template< template< typename... > class Traits, template< typename... > class Func, typename... Rules >
      struct visitor_rt< Traits, Func, rule_list< Rules... > >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            ( visitor_rt< Traits, Func, Rules >::visit( done, args... ), ... );
         }
      };

      template< template< typename... > class Traits, template< typename... > class Func, typename Rule >
      struct visitor_with_subs_rt
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            if( done.emplace( std::type_index( typeid( Rule ) ) ).second ) {
               visit_impl( typename Traits< typename Rule::rule_t >::subs(), args... );
               visitor_with_subs_rt< Traits, Func, typename Traits< typename Rule::rule_t >::subs >::visit( done, args... );
            }
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... >&& /*unused*/, Args&&... args )
         {
            Func< Rule, Subs... >::call( args... );
         }
      };

      template< template< typename... > class Traits, template< typename... > class Func >
      struct visitor_with_subs_rt< Traits, Func, empty_list >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& /*unused*/, Args&&... /*unused*/ )
         {
         }
      };

      template< template< typename... > class Traits, template< typename... > class Func, typename... Rules >
      struct visitor_with_subs_rt< Traits, Func, rule_list< Rules... > >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            ( visitor_with_subs_rt< Traits, Func, Rules >::visit( done, args... ), ... );
         }
      };

   }  // namespace internal

   template< template< typename... > class Traits, template< typename... > class Func, typename Rule, typename... Args >
   void basic_visit_rt( Args&&... args )
   {
      std::set< std::type_index > done;
      internal::visitor_rt< Traits, Func, Rule >::visit( done, args... );
   }

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit_rt( Args&&... args )
   {
      basic_visit_rt< traits, Func, Rule >( args... );
   }

   template< template< typename... > class Traits, template< typename... > class Func, typename Rule, typename... Args >
   void basic_visit_with_subs_rt( Args&&... args )
   {
      std::set< std::type_index > done;
      internal::visitor_with_subs_rt< Traits, Func, Rule >::visit( done, args... );
   }

   template< template< typename... > class Func, typename Rule, typename... Args >
   void visit_with_subs_rt( Args&&... args )
   {
      basic_visit_with_subs_rt< traits, Func, Rule >( args... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
