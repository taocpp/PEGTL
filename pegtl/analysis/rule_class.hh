// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_RULE_CLASS_HH
#define PEGTL_ANALYSIS_RULE_CLASS_HH

namespace pegtl
{
   namespace analysis
   {
      template< typename ... Rules > struct consumes {};
      template< typename ... Rules > struct optional {};

      template< typename ... Rules > struct conjunction {};
      template< typename ... Rules > struct disjunction {};

      template< unsigned Count > struct counting {};
      template< unsigned Count, typename ... Rules > struct repeating {};

   } // analysis

} // pegtl

#endif
