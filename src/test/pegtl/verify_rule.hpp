// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_RULE_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_RULE_HPP

#include <cstdlib>
#include <string>

#include "result_type.hpp"
#include "test_utility.hpp"
#include "verify_impl.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct verify_action_impl
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& /*unused*/, States&&... /*unused*/ )
      {}
   };

   template< typename Rule >
   struct verify_action_impl0
   {
      template< typename... States >
      static void apply0( States&&... /*unused*/ )
      {}
   };

   template< typename Rule, typename Eol = scan::lf_crlf, typename Peek = internal::peek_char >
   void verify_rule( const std::size_t line, const char* file, const std::string& data, const result_type expected, int remain = -1 )
   {
      if( remain < 0 ) {
         remain = ( expected == result_type::success ) ? 0 : int( data.size() );
      }
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
         text_view_input< test::eol_to_lazy_eol< Eol, Peek > > i2( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, nothing >( line, file, data, i2, expected, remain );
      }
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl >( line, file, data, in, expected, remain );
         text_view_input< test::eol_to_lazy_eol< Eol, Peek > > i2( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl >( line, file, data, i2, expected, remain );
      }
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl0 >( line, file, data, in, expected, remain );
         text_view_input< test::eol_to_lazy_eol< Eol, Peek > > i2( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl0 >( line, file, data, i2, expected, remain );
      }
   }

   template< typename Rule >
   void verify_view( const std::size_t line, const char* file, const std::string& data, const result_type expected, int remain = -1 )
   {
      if( remain < 0 ) {
         remain = ( expected == result_type::success ) ? 0 : int( data.size() );
      }
      {
         view_input< void > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
      }
      {
         view_input< void > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl >( line, file, data, in, expected, remain );
      }
      {
         view_input< void > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl0 >( line, file, data, in, expected, remain );
      }
   }

   template< typename Rule, typename Eol = scan::lf_crlf >
   void verify_only( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
   {
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
      }
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl >( line, file, data, in, expected, remain );
      }
      {
         text_view_input< Eol > in( data.data(), data.data() + data.size() );
         verify_impl_one< Rule, verify_action_impl0 >( line, file, data, in, expected, remain );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
