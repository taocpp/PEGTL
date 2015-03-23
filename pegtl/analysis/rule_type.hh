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
         ANY,    // "Success implies consumption" is true; assumes bounded repetition of conjunction of sub-rules.
         OPT,    // "Success implies consumption" is false; assumes bounded repetition of conjunction of sub-rules.

         SEQ,    // Consumption on success depends on non-zero bounded repetition of conjunction of sub-rules.
         SOR,    // Consumption on success depends on non-zero bounded repetition of disjunction of sub-rules.

         PLUS,   // Similar to seq but for non-zero unbounded repetition of conjunction of sub-rules.
         STAR,   // Similar to seq but for optional unbounded repetition of conjunction of sub-rules.

         UNTIL,  // Specific handling of until<> rules.
         IF      // Specific handling of if_then_else<> rule.
      };

   } // analysis

} // pegtl

#endif
