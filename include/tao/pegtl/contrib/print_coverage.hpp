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
      for( const auto& [ k, v ] : r ) {
         os << "RULE \"" << k << "\"\n";
         os << "     start: " << v.start << ", success: " << v.success << ", failure: " << v.failure << ", unwind: " << v.unwind << ", raise: " << v.raise << ", raise_nested: " << v.raise_nested << "\n";
         for( const auto& [ k2, v2 ] : v.branches ) {
            if( v2 ) {
               os << "     BRANCH \"" << k2 << "\"\n";
               os << "            start: " << v2.start << ", success: " << v2.success << ", failure: " << v2.failure << ", unwind: " << v2.unwind << ", raise: " << v2.raise << ", raise_nested: " << v2.raise_nested << "\n";
            }
         }
      }
      return os;
   }

   inline std::ostream& operator<<( std::ostream& os, const std::map< std::string_view, rewind_coverage_info >& r )
   {
      for( const auto& [ k, v ] : r ) {
         os << "RULE \"" << k << "\"\n";
         os << "     prep_rewind: " << v.prep_rewind << ", will_rewind: " << v.will_rewind << ", wont_rewind: " << v.wont_rewind << "\n";
      }
      return os;
   }

   inline std::ostream& operator<<( std::ostream& os, const coverage_result& r )
   {
      os << "\nRULE COVERAGE\n\n";
      os << r.coverage;
      os << "\nREWIND COVERAGE\n\n";
      os << r.rewind;
      return os;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
