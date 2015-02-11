// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_VERIFY_IMPL_HH
#define PEGTL_UNIT_TESTS_VERIFY_IMPL_HH

#include <string>
#include <cstdlib>

#include "../pegtl/error_mode.hh"

#include "test_failed.hh"
#include "result_help.hh"
#include "verify_help.hh"

namespace pegtl
{
   template< typename Rule, error_mode M >
   void verify_impl( const std::size_t line, const char * file, const std::string & data, const result_type result, const std::size_t remain )
   {
      pegtl::input i( line, 0, data.data(), data.data() + data.size(), file );

      const result_type expected = result_help( M, result );
      const result_type received = verify_help< Rule, M >( i );

      if ( ( received == expected ) && ( ( received == result_type::GLOBAL_FAILURE ) || ( i.size() == remain ) ) ) {
         return;
      }
      TEST_FAILED( "input data [ '" << data << "' ] result received/expected [ " << received << " / " << expected << " ] remain received/expected [ " << i.size() << " / " << remain << " ]" );
   }

} // pegtl

#endif
