// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <set>
#include <string>
#include <type_traits>
#include <typeindex>

#include <tao/pegtl.hpp>
#include <tao/pegtl/traits.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, template< typename... > class Func >
      struct visitor_rt
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            if( done.emplace( std::type_index( typeid( Rule ) ) ).second ) {
               Func< Rule >::call( args... );
               visitor_rt< typename traits< typename Rule::rule_t >::subs, Func >::visit( done, args... );
            }
         }
      };

      template< template< typename... > class Func >
      struct visitor_rt< empty_list, Func >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& /*unused*/, Args&&... /*unused*/ )
         {
         }
      };

      template< typename... Rules, template< typename... > class Func >
      struct visitor_rt< rule_list< Rules... >, Func >
      {
         template< typename... Args >
         static void visit( std::set< std::type_index >& done, Args&&... args )
         {
            ( visitor_rt< Rules, Func >::visit( done, args... ), ... );
         }
      };

   }  // namespace internal

   template< typename Rule, template< typename... > class Func, typename... Args >
   void visit_rt( Args&&... args )
   {
      std::set< std::type_index > done;
      internal::visitor_rt< Rule, Func >::visit( done, args... );
   }

   template< typename Rule >
   struct printer_rt
   {
      static void call()
      {
         std::cout << internal::demangle< Rule >() << std::endl;
      }
   };

   template< typename Rule >
   void print_rt()
   {
      visit_rt< Rule, printer_rt >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int, char** )  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_rt< tao::pegtl::json::text >();
   return 0;
}
