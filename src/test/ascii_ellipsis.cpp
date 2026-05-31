// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ellipsis, internal::ascii_string< '.', '.', '.' > >();

      verify_analyze< ellipsis >( __LINE__, __FILE__, true, false );

      verify_rule< ellipsis >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< ellipsis >( __LINE__, __FILE__, ".", result_type::local_failure );
      verify_rule< ellipsis >( __LINE__, __FILE__, "..", result_type::local_failure );
      verify_rule< ellipsis >( __LINE__, __FILE__, ". .", result_type::local_failure );
      verify_rule< ellipsis >( __LINE__, __FILE__, "...", result_type::success );
      verify_rule< ellipsis >( __LINE__, __FILE__, "....", result_type::success, 1 );
      verify_rule< ellipsis >( __LINE__, __FILE__, "...a", result_type::success, 1 );
      verify_rule< ellipsis >( __LINE__, __FILE__, "a...", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
