// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/icu/utf32.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      // verify_analyze< utf32::icu::alphabetic >( __LINE__, __FILE__, true, false );

      // verify_rule< utf32::icu::alphabetic >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      // verify_rule< utf32::icu::alphabetic >( __LINE__, __FILE__, std::string( "a\0\0", 4 ), result_type::success );  // LE
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
