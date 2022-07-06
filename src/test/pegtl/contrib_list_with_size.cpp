// Copyright (c) 2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/contrib/list_with_size.hpp>

namespace tao::pegtl
{
   void unit_test()
   {
      verify_analyze< list_with_size< 3, alpha, digit > >( __LINE__, __FILE__, true, false );
      verify_analyze< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, true, false );
      verify_analyze< list_with_size< 3, alpha, digit, opt< blank > > >( __LINE__, __FILE__, true, true );

      verify_rule< list_with_size< 3, alpha, digit > >( __LINE__, __FILE__, "a1b2", result_type::local_failure );
      verify_rule< list_with_size< 3, alpha, digit > >( __LINE__, __FILE__, "a1b2c", result_type::success );
      verify_rule< list_with_size< 3, alpha, digit > >( __LINE__, __FILE__, "a1b2c3", result_type::success, 1 );

      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a1b2", result_type::local_failure );
      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a1b2c", result_type::success );
      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a1b2c3", result_type::success, 1 );

      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a 1 b 2 ", result_type::local_failure );
      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a 1 b 2 c ", result_type::success, 1 );
      verify_rule< list_with_size< 3, alpha, digit, blank > >( __LINE__, __FILE__, "a 1 b 2 c 3", result_type::success, 2 );
   }

}  // namespace tao::pegtl

#include "main.hpp"
