// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_IMPL_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_IMPL_HPP

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

#include <tao/pegtl/apply_mode.hpp>
#include <tao/pegtl/memory_input.hpp>
#include <tao/pegtl/normal.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/rewind_mode.hpp>

#include "result_type.hpp"
#include "test_failed.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule,
                template< typename... > class Action,
                typename Input >
      result_type verify_impl_impl_impl( Input& i )
      {
         try {
            if( normal< Rule >::template match< apply_mode::ACTION, rewind_mode::REQUIRED, Action, normal >( i ) ) {
               return result_type::SUCCESS;
            }
            return result_type::LOCAL_FAILURE;
         }
         catch( const std::exception& ) {
            return result_type::GLOBAL_FAILURE;
         }
         catch( ... ) {
            throw std::runtime_error( "code should be unreachable" );  // LCOV_EXCL_LINE
         }
      }

      template< typename Rule, typename Eol, template< typename... > class Action >
      void verify_impl_impl( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
      {
         basic_memory_input< Eol > i( { 0, line, 0, data.data() }, data.data() + data.size(), file );

         const result_type received = verify_impl_impl_impl< Rule, Action >( i );

         if( ( received == expected ) && ( ( received == result_type::GLOBAL_FAILURE ) || ( i.size( 999999999 ) == remain ) ) ) {
            return;
         }
         TAOCPP_PEGTL_TEST_FAILED( "input data [ '" << data << "' ] result received/expected [ " << received << " / " << expected << " ] remain received/expected [ " << i.size( 999999999 ) << " / " << remain << " ]" );
      }

      template< typename Rule >
      struct verify_action_impl
      {
         template< typename Input, typename... States >
         static void apply( const Input&, States&&... )
         {
         }
      };

      template< typename Rule >
      struct verify_action_impl0
      {
         template< typename... States >
         static void apply0( States&&... )
         {
         }
      };

      template< typename Rule, typename Eol >
      void verify_impl( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
      {
         verify_impl_impl< Rule, Eol, nothing >( line, file, data, expected, remain );
         verify_impl_impl< Rule, Eol, verify_action_impl >( line, file, data, expected, remain );
         verify_impl_impl< Rule, Eol, verify_action_impl0 >( line, file, data, expected, remain );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
