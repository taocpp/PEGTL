// Copyright (c) 2020-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

//#include <tao/pegtl/contrib/icu/utf8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      //      verify_analyze< utf8::icu::alphabetic >( __LINE__, __FILE__, true, false );

      //      verify_rule< utf8::icu::alphabetic >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      //      verify_rule< utf8::icu::alphabetic >( __LINE__, __FILE__, "a", result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
