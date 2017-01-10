// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_VERIFY_HELP_HH
#define PEGTL_UNIT_TESTS_VERIFY_HELP_HH

#include <cassert>
#include <stdexcept>

#include "result_type.hh"

#include "../pegtl/normal.hh"
#include "../pegtl/nothing.hh"
#include "../pegtl/apply_mode.hh"
#include "../pegtl/rewind_mode.hh"

namespace pegtl
{
   template< typename Rule, typename Input >
   result_type verify_help( Input & i )
   {
      try {
         if ( normal< Rule >::template match< apply_mode::ACTION, rewind_mode::REQUIRED, nothing, normal >( i ) ) {
            return result_type::SUCCESS;
         }
         return result_type::LOCAL_FAILURE;
      }
      catch ( const std::exception & ) {
         return result_type::GLOBAL_FAILURE;
      }
      catch ( ... ) {
         throw std::runtime_error( "code should be unreachable" );  // LCOV_EXCL_LINE
      }
   }

} // pegtl

#endif
