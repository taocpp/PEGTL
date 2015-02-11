// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_RESULT_HELP_HH
#define PEGTL_UNIT_TESTS_RESULT_HELP_HH

#include <cassert>

#include "result_type.hh"

namespace pegtl
{
   result_type result_help( const error_mode error, const result_type result )
   {
      switch ( result ) {
         case result_type::SUCCESS:
            return result;
         case result_type::LOCAL_FAILURE:
            return ( error == error_mode::RETURN ) ? result_type::LOCAL_FAILURE : result_type::GLOBAL_FAILURE;
         case result_type::GLOBAL_FAILURE:
            return result;
      }
      assert( false );
   }

} // pegtl

#endif
