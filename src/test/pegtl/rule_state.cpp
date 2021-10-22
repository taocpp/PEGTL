// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

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

   template< typename... Rules >
   using test_state_rule = state< test_state_state, Rules... >;

   void unit_test()
   {
      verify_meta< state< test_state_state >, internal::success >();
      verify_meta< state< test_state_state, any >, internal::state< test_state_state, any >, any >();
      verify_meta< state< test_state_state, alpha, digit >, internal::state< test_state_state, internal::seq< alpha, digit > >, internal::seq< alpha, digit > >();

      verify_seqs< test_state_rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
