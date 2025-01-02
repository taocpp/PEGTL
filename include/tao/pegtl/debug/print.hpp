// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_PRINT_HPP
#define TAO_PEGTL_DEBUG_PRINT_HPP

#include <ostream>

#include "../config.hpp"
#include "../demangle.hpp"
#include "../type_list.hpp"

#include "visit.hpp"

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

   }  // namespace internal

   template< typename Grammar >
   void print_names( std::ostream& os )
   {
      TAO_PEGTL_NAMESPACE::visit< Grammar, internal::print_names >( os );
   }

   template< typename Grammar >
   void print_debug( std::ostream& os )
   {
      TAO_PEGTL_NAMESPACE::visit< Grammar, internal::print_debug >( os );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
