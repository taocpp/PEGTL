// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_RULE_HPP  // NOLINT
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_RULE_HPP

#include <cstdlib>
#include <string>

#include <tao/pegtl/eol.hpp>
#include <tao/pegtl/memory_input.hpp>
#include <tao/pegtl/tracking_mode.hpp>

#include "result_type.hpp"
#include "verify_impl.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule >
      struct verify_action_impl
      {
         template< typename Input, typename... States >
         static void apply( const Input& /*unused*/, States&&... /*unused*/ )
         {
         }
      };

      template< typename Rule >
      struct verify_action_impl0
      {
         template< typename... States >
         static void apply0( States&&... /*unused*/ )
         {
         }
      };

      template< typename Rule, typename Eol = eol::lf_crlf >
      void verify_rule( const std::size_t line, const char* file, const std::string& data, const result_type expected, std::size_t remain = 0 )
      {
         remain = ( expected == result_type::SUCCESS ) ? remain : data.size();
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
            memory_input< tracking_mode::LAZY, Eol > i2( data.data(), data.data() + data.size(), file );
            verify_impl_one< Rule, nothing >( line, file, data, i2, expected, remain );
         }
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, verify_action_impl >( line, file, data, in, expected, remain );
            memory_input< tracking_mode::LAZY, Eol > i2( data.data(), data.data() + data.size(), file );
            verify_impl_one< Rule, verify_action_impl >( line, file, data, i2, expected, remain );
         }
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, verify_action_impl0 >( line, file, data, in, expected, remain );
            memory_input< tracking_mode::LAZY, Eol > i2( data.data(), data.data() + data.size(), file );
            verify_impl_one< Rule, verify_action_impl0 >( line, file, data, i2, expected, remain );
         }
      }

      template< typename Rule, typename Eol = eol::lf_crlf >
      void verify_only( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
      {
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
         }
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, verify_action_impl >( line, file, data, in, expected, remain );
         }
         {
            memory_input< tracking_mode::IMMEDIATE, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
            verify_impl_one< Rule, verify_action_impl0 >( line, file, data, in, expected, remain );
         }
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
