// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/example/lua53.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using grammar = lua53::grammar;

   void unit_test()
   {
      verify_rule< grammar >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "return 1", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "local x = 1\nx = x + 2", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "function f(a, b) return a + b end", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "if true then local x = 1 else local x = 2 end", result_type::success );

      verify_rule< grammar >( __LINE__, __FILE__, "local 1", result_type::global_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "function () end", result_type::global_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
