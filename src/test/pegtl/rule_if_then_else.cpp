// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ifmt.hpp"

namespace tao::pegtl
{
   void unit_test()
   {
      verify_meta< if_then_else< digit, alpha, print >, internal::if_then_else< digit, alpha, print >, digit, alpha, print >();

      verify_ifmt< if_then_else >();
   }

}  // namespace tao::pegtl

#include "main.hpp"
