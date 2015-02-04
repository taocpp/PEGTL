// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_RULE_TYPE_HH
#define PEGTL_ANALYSIS_RULE_TYPE_HH

namespace pegtl
{
   namespace analysis
   {
      enum class rule_type : char
      {
         CONSUMES,
         OPTIONAL,

         CONJUNCTION,
         DISJUNCTION,

         RULE_UNTIL,
         RULE_IF_THEN_ELSE
      };

   } // analysis

} // pegtl

#endif
