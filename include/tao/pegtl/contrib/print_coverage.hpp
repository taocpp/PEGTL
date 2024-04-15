// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_PRINT_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_PRINT_COVERAGE_HPP

#include <ostream>
#include <string_view>

#include "../config.hpp"

#include "coverage.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   inline std::ostream& operator<<( std::ostream& os, const std::map< std::string_view, coverage_entry >& r )
   {
      os << "[\n";
      bool f = true;
      for( const auto& [ k, v ] : r ) {
         if( f ) {
            f = false;
         }
         else {
            os << ",\n";
         }
         os << "  {\n"
            << "    \"rule\": \"" << k << "\",\n"
            << "    \"start\": " << v.start << ", \"success\": " << v.success << ", \"failure\": " << v.failure << ", \"unwind\": " << v.unwind << ", \"raise\": " << v.raise << ", \"raise_nested\": " << v.raise_nested << ",\n";
         if( v.branches.empty() ) {
            os << "\n";
         }
         else {
            os << ",\n    \"branches\": [\n";
            bool f2 = true;
            for( const auto& [ k2, v2 ] : v.branches ) {
               if( f2 ) {
                  f2 = false;
               }
               else {
                  os << ",\n";
               }
               os << "      { \"branch\": \"" << k2 << "\", \"start\": " << v2.start << ", \"success\": " << v2.success << ", \"failure\": " << v2.failure << ", \"unwind\": " << v2.unwind << ", \"raise\": " << v2.raise << ", \"raise_nested\": " << v2.raise_nested << " }";
            }
            os << "\n    ]\n";
         }
         os << "  }";
      }
      os << "\n";
      os << "]\n";
      return os;
   }

   inline std::ostream& operator<<( std::ostream& os, const std::map< std::string_view, rewind_coverage_info >& r )
   {
      os << "[ {\n";
      bool f = true;
      for( const auto& [ k, v ] : r ) {
         if( f ) {
            f = false;
         }
         else {
            os << "}, {\n";
         }
         os << "  \"rule\": \"" << k << "\",\n"
            << "  \"prep_rewind\": " << v.prep_rewind << ", \"will_rewind\": " << v.will_rewind << ", \"wont_rewind\": " << v.wont_rewind << "\n";
      }
      os << "} ]\n";
      return os;
   }

   inline std::ostream& operator<<( std::ostream& os, const coverage_result& r )
   {
      os << "RULE COVERAGE\n";
      os << r.coverage;
      os << "REWIND COVERAGE\n";
      os << r.rewind;
      return os;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
