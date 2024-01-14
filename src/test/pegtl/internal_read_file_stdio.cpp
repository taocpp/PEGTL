// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/read_file_stdio.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const auto data = internal::read_file_stdio( "src/test/pegtl/data/duseltronik.txt" ).read_string();
      TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
