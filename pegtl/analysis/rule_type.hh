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
         CONJUNCTION,
         DISJUNCTION,

         CAN_SUCCEED_WITHOUT_CONSUME,
         ALWAYS_CONSUMES_WHEN_SUCCEEDS,

         RULE_UNTIL,
         RULE_IF_THEN_ELSE,
         RULE_IF_MUST_ELSE
      };

   } // analysis

} // pegtl

#endif
