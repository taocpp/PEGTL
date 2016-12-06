// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_VERIFY_IMPL_HH
#define PEGTL_UNIT_TESTS_VERIFY_IMPL_HH

#include <cstddef>
#include <string>

#include "../pegtl/memory_input.hh"

#include "test_failed.hh"
#include "verify_help.hh"

namespace pegtl
{
   template< typename Rule, typename Eol >
   void verify_impl( const std::size_t line, const char * file, const std::string & data, const result_type expected, const std::size_t remain )
   {
      basic_memory_input< Eol > i( 0, line, 0, data.data(), data.data() + data.size(), file );

      const result_type received = verify_help< Rule >( i );

      if ( ( received == expected ) && ( ( received == result_type::GLOBAL_FAILURE ) || ( i.size( 999999999 ) == remain ) ) ) {
         return;
      }
      TEST_FAILED( "input data [ '" << data << "' ] result received/expected [ " << received << " / " << expected << " ] remain received/expected [ " << i.size( 999999999 ) << " / " << remain << " ]" );
   }

} // pegtl

#endif
