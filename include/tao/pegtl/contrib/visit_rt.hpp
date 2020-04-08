// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_VISIT_RT_HPP
#define TAO_PEGTL_CONTRIB_VISIT_RT_HPP

#include <set>
#include <string_view>

#include "../config.hpp"
#include "../rule_list.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< template< typename... > class Func, typename Rule >
      struct visitor_rt
      {
         template< typename... Args >
         static void visit( std::set< std::string_view >& done, Args&&... args )
         {
            if( done.emplace( demangle< Rule >() ).second ) {
               visit_impl( typename Rule::subs_t(), args... );
               visitor_rt< Func, typename Rule::subs_t >::visit( done, args... );
            }
         }

      private:
         template< typename... Subs, typename... Args >
         static void visit_impl( rule_list< Subs... >&& /*unused*/, Args&&... args )
         {
            Func< Rule, Subs... >::call( args... );
         }
      };

      template< template< typename... > class Func >
      struct visitor_rt< Func, empty_list >
      {
         template< typename... Args >
         static void visit( std::set< std::string_view >& /*unused*/, Args&&... /*unused*/ )
         {
         }
      };

      template< template< typename... > class Func, typename... Rules >
      struct visitor_rt< Func, rule_list< Rules... > >
      {
         template< typename... Args >
         static void visit( std::set< std::string_view >& done, Args&&... args )
         {
            ( visitor_rt< Func, Rules >::visit( done, args... ), ... );
         }
      };

   }  // namespace internal

   template< template< typename... > class Func, typename Rule, typename... Args >
   std::size_t visit_rt( Args&&... args )
   {
      std::set< std::string_view > done;
      internal::visitor_rt< Func, Rule >::visit( done, args... );
      return done.size();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
