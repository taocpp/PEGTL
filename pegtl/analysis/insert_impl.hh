// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_INSERT_IMPL_HH
#define PEGTL_ANALYSIS_INSERT_IMPL_HH

#include "rule_info.hh"
#include "insert_rule.hh"
#include "grammar_info.hh"

namespace pegtl
{
   namespace analysis
   {
      template< typename ... > struct insert_impl;

      template<> struct insert_impl<>
      {
         static void insert( grammar_info &, rule_info & )
         { }
      };

      template< typename Rule, typename ... Rules > struct insert_impl< Rule, Rules ... >
      {
         static void insert( grammar_info & g, rule_info & r )
         {
            r.rules.push_back( insert_rule< Rule >( g, static_cast< const typename Rule::analyze_t * >( nullptr ) ) );
            insert_impl< Rules ... >::insert( g, r );
         }
      };

   } // analysis

} // pegtl

#endif
