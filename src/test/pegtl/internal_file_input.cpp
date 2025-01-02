// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const internal::file_input in( "src/test/pegtl/data/duseltronik.txt" );
      const std::string data( in.start(), in.end() );
      TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
