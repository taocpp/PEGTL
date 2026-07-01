// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_ifmt.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< if_then_else< digit, alpha, print >, internal::if_then_else< digit, alpha, print >, digit, alpha, print >();

      verify_ctrl_enabled< if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ab" );
      verify_ctrl_disabled< internal::if_then_else< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ab" );

      verify_ifmt< if_then_else >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
