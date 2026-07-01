// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< seq<>, internal::success >();
      verify_meta< seq< alpha >, internal::seq< alpha >, alpha >();
      verify_meta< seq< alpha, digit >, internal::seq< alpha, digit >, alpha, digit >();

      verify_seqs< seq >();

      verify_ctrl_enabled< seq<> >( __LINE__, __FILE__, "" );
      verify_ctrl_disabled< internal::seq<> >( __LINE__, __FILE__, "" );
      verify_ctrl_enabled< seq< alpha > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::seq< alpha > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< seq< alpha, digit > >( __LINE__, __FILE__, "a1" );
      verify_ctrl_disabled< internal::seq< alpha, digit > >( __LINE__, __FILE__, "a1" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
