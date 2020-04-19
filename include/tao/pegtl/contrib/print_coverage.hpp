// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_PRINT_COVERAGE_HPP

#include <ostream>

#include "coverage.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // output is JSON
   inline void print_coverage( std::ostream& os, const coverage_state& state )
   {
      os << "{\n"
         << "  \"grammar\": \"" << state.grammar << "\",\n"
         << "  \"source\": \"" << state.source << "\",\n"
         << "  \"result\": " << ( state.result ? "true" : "false" ) << ",\n"
         << "  \"coverage\":\n"
         << "  [\n";
      bool f = true;
      for( const auto& [ k, v ] : state.map ) {
         if( f ) {
            f = false;
         }
         else {
            os << ",\n";
         }
         os << "    {\n"
            << "      \"rule\": \"" << k << "\",\n"
            << "      \"start\": " << v.start << ", \"success\": " << v.success << ", \"local_failure\": " << v.local_failure << ", \"global_failure\": " << v.global_failure << ", \"raise\": " << v.raise << ",\n";
         if( v.branches.empty() ) {
            os << "      \"branches\": []\n";
         }
         else {
            os << "      \"branches\": [\n";
            bool f2 = true;
            for( const auto& [ k2, v2 ] : v.branches ) {
               if( f2 ) {
                  f2 = false;
               }
               else {
                  os << ",\n";
               }
               os << "        { \"branch\": \"" << k2 << "\", \"start\": " << v2.start << ", \"success\": " << v2.success << ", \"local_failure\": " << v2.local_failure << ", \"global_failure\": " << v2.global_failure << ", \"raise\": " << v2.raise << " }";
            }
            os << "\n      ]\n";
         }
         os << "    }";
      }
      os << "\n"
         << "  ]\n"
         << "}\n";
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
