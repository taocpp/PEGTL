// Copyright (c) 2015-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using lazy_input = lazy_file_input< ascii::lf_crlf >;
      using text_input = text_file_input< ascii::lf_crlf >;

      verify_file< lazy_input >();
      verify_file< text_input >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
