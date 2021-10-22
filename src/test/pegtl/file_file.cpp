// Copyright (c) 2015-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_file< file_input<> >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
