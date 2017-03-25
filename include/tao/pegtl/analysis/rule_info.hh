// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_ANALYSIS_RULE_INFO_HH
#define TAOCPP_PEGTL_INCLUDE_ANALYSIS_RULE_INFO_HH

#include <string>
#include <vector>

#include "../config.hh"

#include "rule_type.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace analysis
      {
         struct rule_info
         {
            explicit
            rule_info( const rule_type in_type )
                  : type( in_type )
            { }

            rule_type type;
            std::vector< std::string > rules;
         };

      } // namespace analysis

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
