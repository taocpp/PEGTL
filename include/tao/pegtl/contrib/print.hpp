// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_HPP
#define TAO_PEGTL_CONTRIB_PRINT_HPP

#include <cassert>
#include <iomanip>
#include <ios>
#include <ostream>
#include <string_view>

#include "../config.hpp"
#include "../demangle.hpp"
#include "../visit.hpp"

#include "print_rules_traits.hpp"

#include "internal/print_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Name >
      struct print_names
      {
         static void visit( std::ostream& os )
         {
            os << demangle< Name >() << '\n';
         }
      };

      template< typename Name >
      struct print_debug
      {
         static void visit( std::ostream& os )
         {
            const auto first = demangle< Name >();
            os << first << '\n';

            const auto second = demangle< typename Name::rule_t >();
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

      template< typename Rule >
      struct print_rules
      {
         static void visit( std::ostream& os, const std::string_view prefix, const int width )
         {
            const std::string_view rule = demangle< Rule >();
            if( is_in_namespace( rule, prefix ) ) {
               os << std::right << std::setw( width ) << rule.substr( prefix.size() ) << " = ";
               print_rules_traits< typename Rule::rule_t >::print( os, prefix );
               os << '\n';
            }
         }
      };

   }  // namespace internal

   template< typename Grammar >
   void print_names( std::ostream& os )
   {
      visit< Grammar, internal::print_names >( os );
   }

   template< typename Grammar >
   void print_debug( std::ostream& os )
   {
      visit< Grammar, internal::print_debug >( os );
   }

   template< typename Grammar >
   void print_rules( std::ostream& os, const std::string_view prefix, const int width = 21 )
   {
      assert( !prefix.empty() );  // TODO: Something more fitting.
      visit< Grammar, internal::print_rules >( os, prefix, width );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
