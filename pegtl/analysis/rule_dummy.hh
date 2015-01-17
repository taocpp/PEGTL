// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_RULE_DUMMY_HH
#define PEGTL_ANALYSIS_RULE_DUMMY_HH

namespace pegtl
{
   namespace analysis
   {
      template< typename Rule > struct rule_dummy
      {
         explicit
         rule_dummy( const bool verbose )
               : verbose( verbose )
         { }

         bool verbose;
      };

   } // analysis

} // pegtl

#endif
