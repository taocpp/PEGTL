// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_GRAMMAR_INFO_HH
#define PEGTL_ANALYSIS_GRAMMAR_INFO_HH

#include <map>
#include <string>
#include <utility>

#include "../config.hh"
#include "../internal/demangle.hh"

#include "rule_info.hh"

namespace PEGTL_NAMESPACE
{
   namespace analysis
   {
      struct grammar_info
      {
         using map_t = std::map< std::string, rule_info >;
         map_t map;

         template< typename Name >
         std::pair< map_t::iterator, bool > insert( const rule_type type )
         {
            return map.insert( map_t::value_type( internal::demangle< Name >(), rule_info( type ) ) );
         }
      };

   } // namespace analysis

} // namespace PEGTL_NAMESPACE

#endif
