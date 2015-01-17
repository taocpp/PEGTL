// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_GRAMMAR_INFO_HH
#define PEGTL_ANALYSIS_GRAMMAR_INFO_HH

#include <map>
#include <utility>

#include "../internal/demangle.hh"

#include "rule_info.hh"

namespace pegtl
{
   namespace analysis
   {
      struct grammar_info
      {
         template< typename Rule >
         std::pair< std::map< std::string, rule_info >::iterator, bool > insert( const rule_type type )
         {
            return map.insert( std::pair< std::string, rule_info >( internal::demangle< Rule >(), rule_info( type ) ) );
         }

         std::map< std::string, rule_info > map;
      };

   } // analysis

} // pegtl

#endif
