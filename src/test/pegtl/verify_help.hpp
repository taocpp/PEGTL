// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_HELP_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_HELP_HPP

#include <cassert>
#include <stdexcept>

#include "result_type.hpp"

#include <tao/pegtl/normal.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/apply_mode.hpp>
#include <tao/pegtl/rewind_mode.hpp>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
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

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
