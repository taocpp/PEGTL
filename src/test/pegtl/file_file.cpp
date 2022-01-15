// Copyright (c) 2015-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_file.hpp"

namespace tao::pegtl
{
   void unit_test()
   {
      verify_file< file_input<> >();
   }

}  // namespace tao::pegtl

#include "main.hpp"
