// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_ANALYSIS_INSERT_RULES_HH
#define TAOCPP_PEGTL_INCLUDE_ANALYSIS_INSERT_RULES_HH

#include "../config.hh"

#include "rule_info.hh"
#include "grammar_info.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace analysis
   {
      template< typename ... > struct insert_rules;

      template<>
      struct insert_rules<>
      {
         static void insert( grammar_info &, rule_info & )
         { }
      };

      template< typename Rule, typename ... Rules >
      struct insert_rules< Rule, Rules ... >
      {
         static void insert( grammar_info & g, rule_info & r )
         {
            r.rules.push_back( Rule::analyze_t::template insert< Rule >( g ) );
            insert_rules< Rules ... >::insert( g, r );
         }
      };

   } // namespace analysis

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
