// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_GENERIC_HH
#define PEGTL_ANALYSIS_GENERIC_HH

#include "../config.hh"

#include "rule_type.hh"
#include "insert_rules.hh"
#include "grammar_info.hh"

namespace PEGTL_NAMESPACE
{
   namespace analysis
   {
      template< rule_type Type, typename ... Rules >
      struct generic
      {
         template< typename Name >
         static std::string insert( grammar_info & g )
         {
            const auto r = g.insert< Name >( Type );
            if ( r.second ) {
               insert_rules< Rules ... >::insert( g, r.first->second );
            }
            return r.first->first;
         }
      };

   } // namespace analysis

} // namespace PEGTL_NAMESPACE

#endif
