// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_IMPL_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_IMPL_HPP

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

#include "result_type.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, template< typename... > class Action, typename ParseInput >
   result_type verify_impl_two( ParseInput& in )
   {
#if defined( __cpp_exceptions )
      try {
         if( normal< Rule >::template match< apply_mode::action, rewind_mode::required, Action, normal >( in ) ) {
            return result_type::success;
         }
         return result_type::local_failure;
      }
      catch( const std::exception& ) {
         return result_type::global_failure;
      }
      // LCOV_EXCL_START
      catch( ... ) {
         TAO_PEGTL_TEST_UNREACHABLE;
      }
      // LCOV_EXCL_STOP
#else
      if( normal< Rule >::template match< apply_mode::action, rewind_mode::required, Action, normal >( in ) ) {
         return result_type::success;
      }
      return result_type::local_failure;
#endif
   }

   template< typename Rule, template< typename... > class Action, typename ParseInput >
   void verify_impl_one( const std::size_t line, const char* file, const std::string& data, ParseInput& in, const result_type expected, const std::size_t remain )
   {
      const result_type received = verify_impl_two< Rule, Action >( in );

      if( ( received == expected ) && ( ( received == result_type::global_failure ) || ( in.size() == remain ) ) ) {
         return;
      }
      TAO_PEGTL_TEST_FAILED( "input data [ '" << data << "' ] result received/expected [ " << received << " / " << expected << " ] remain received/expected [ " << in.size() << " / " << remain << " ]" );  // LCOV_EXCL_LINE
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
