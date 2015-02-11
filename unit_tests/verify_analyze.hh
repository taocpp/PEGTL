// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_VERIFY_ANALYZE_HH
#define PEGTL_UNIT_TESTS_VERIFY_ANALYZE_HH

#include <pegtl/analyze.hh>

#include "test_failed.hh"

namespace pegtl
{
   template< typename Rule >
   void verify_analyze( const unsigned line, const char * file, const bool consumes, const bool problems = false, const bool verbose = false )
   {
      analysis::analyze_cycles< Rule > a( verbose );

      if ( problems != bool( a.problems() ) ) {
         TEST_FAILED( "analyze -- problems received/expected [ " << ( ! problems ) << " / " << problems << " ]" );
      }
      if ( consumes != bool( a.template consumes< Rule >() ) ) {
         TEST_FAILED( "analyze -- consumes received/expected [ " << ( ! consumes ) << " / " << consumes << " ]" );
      }
   }

} // pegtl

#endif
