// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_HPP
#define TAO_PEGTL_CONTRIB_PRINT_HPP

#include <iostream>

#include "../config.hpp"
#include "../visit.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Name >
      struct print_rules_impl
      {
         static void visit( std::ostream& os )
         {
            os << demangle< Name >() << std::endl;
         }
      };

      template< typename Name >
      struct print_sub_rules_impl
      {
         static void visit( std::ostream& os )
         {
            print_impl( os, typename Name::subs_t() );
         }

      private:
         template< typename... Rules >
         static void print_impl( std::ostream& os, type_list< Rules... > )
         {
            const auto first = demangle< Name >();
            const auto second = demangle< typename Name::rule_t >();

            os << first << std::endl;

            if( first != second ) {
               os << " (aka) " << second << std::endl;
            }
            ( print_rule< Rules >( os ), ... );
            os << std::endl;
         }

         template< typename Rule >
         static void print_rule( std::ostream& os )
         {
            os << " (sub) " << demangle< Rule >() << std::endl;
         }
      };

   }  // namespace internal

   template< typename Grammar >
   void print_rules( std::ostream& os = std::cout )
   {
      visit< Grammar, internal::print_rules_impl >( os );
   }

   template< typename Grammar >
   void print_sub_rules( std::ostream& os = std::cout )
   {
      visit< Grammar, internal::print_sub_rules_impl >( os );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
