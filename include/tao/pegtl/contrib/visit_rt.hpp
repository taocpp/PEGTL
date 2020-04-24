// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_VISIT_RT_HPP
#define TAO_PEGTL_CONTRIB_VISIT_RT_HPP

#include <set>
#include <string_view>

#include "../config.hpp"
#include "../demangle.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< template< typename... > class Func, typename... Rules >
      struct visitor_rt
      {
         template< typename... Args >
         static void visit( std::set< std::string_view >& done, Args&&... args )
         {
            ( visit_rule< Rules >( typename Rules::subs_t(), done, args... ), ... );
         }

      private:
         template< typename Rule, typename... Subs, typename... Args >
         static void visit_rule( type_list< Subs... > /*unused*/, std::set< std::string_view >& done, Args&&... args )
         {
            if( done.emplace( demangle< Rule >() ).second ) {
               Func< Rule >::visit( args... );
               visitor_rt< Func, Subs... >::visit( done, args... );
            }
         }
      };

   }  // namespace internal

   template< typename Rule, template< typename... > class Func, typename... Args >
   std::size_t visit_rt( Args&&... args )
   {
      std::set< std::string_view > done;
      internal::visitor_rt< Func, Rule >::visit( done, args... );
      return done.size();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
