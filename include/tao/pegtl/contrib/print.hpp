// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_HPP
#define TAO_PEGTL_CONTRIB_PRINT_HPP

#include <ostream>

#include "../config.hpp"
#include "../visit.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Name >
      struct print_rules
      {
         static void visit( std::ostream& os )
         {
            os << demangle< Name >() << '\n';
         }
      };

      template< typename Name >
      struct print_sub_rules
      {
         static void visit( std::ostream& os )
         {
            constexpr auto first = demangle< Name >();
            os << first << '\n';

            constexpr auto second = demangle< typename Name::rule_t >();
            if( first != second ) {
               os << " (aka) " << second << '\n';
            }

            print_subs( os, typename Name::subs_t() );

            os << '\n';
         }

      private:
         template< typename... Rules >
         static void print_subs( std::ostream& os, type_list< Rules... > /*unused*/ )
         {
            ( print_sub< Rules >( os ), ... );
         }

         template< typename Rule >
         static void print_sub( std::ostream& os )
         {
            os << " (sub) " << demangle< Rule >() << '\n';
         }
      };

   }  // namespace internal

   template< typename Grammar >
   void print_rules( std::ostream& os )
   {
      visit< Grammar, internal::print_rules >( os );
   }

   template< typename Grammar >
   void print_sub_rules( std::ostream& os )
   {
      visit< Grammar, internal::print_sub_rules >( os );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
