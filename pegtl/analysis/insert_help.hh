// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_INSERT_HELP_HH
#define PEGTL_ANALYSIS_INSERT_HELP_HH

#include "rule_type.hh"
#include "grammar_info.hh"

namespace pegtl
{
   namespace analysis
   {
      template< typename ... > struct insert_impl;

      template< typename Name, typename ... Rules >
      std::string insert_help( grammar_info & g, const rule_type t )
      {
         const auto r = g.insert< Name >( t );
         if ( r.second ) {
            insert_impl< Rules ... >::insert( g, r.first->second );
         }
         return r.first->first;
      }

   } // analysis

} // pegtl

#endif
