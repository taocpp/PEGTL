// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct test_state_state
   {
      template< typename ParseInput >
      explicit test_state_state( const ParseInput& /*unused*/ )
      {}

      template< typename ParseInput >
      void success( const ParseInput& /*unused*/ ) const
      {}
   };

   struct verify_state_state
   {
      template< typename ParseInput, typename... States >
      explicit verify_state_state( const ParseInput& /*unused*/, States&&... /*unused*/ )
      {}

      template< typename ParseInput, typename... States >
      void success( const ParseInput& /*unused*/, States&&... /*unused*/ ) const
      {}
   };

   template< typename... Rules >
   using test_state_rule = state< test_state_state, Rules... >;

   void unit_test()
   {
      verify_meta< state< test_state_state >, internal::success >();
      verify_meta< state< test_state_state, any >, internal::state< test_state_state, any >, any >();
      verify_meta< state< test_state_state, alpha, digit >, internal::state< test_state_state, internal::seq< alpha, digit > >, internal::seq< alpha, digit > >();

      verify_seqs< test_state_rule >();

      verify_ctrl_enabled< state< verify_state_state > >( __LINE__, __FILE__, "" );
      verify_ctrl_disabled< internal::state< verify_state_state > >( __LINE__, __FILE__, "" );
      verify_ctrl_enabled< state< verify_state_state, alpha > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::state< verify_state_state, alpha > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< state< verify_state_state, alpha, digit > >( __LINE__, __FILE__, "a1" );
      verify_ctrl_disabled< internal::state< verify_state_state, internal::seq< alpha, digit > > >( __LINE__, __FILE__, "a1" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
