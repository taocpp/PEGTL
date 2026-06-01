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
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/example/proto3.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using grammar = proto3::proto;

   void unit_test()
   {
      verify_analyze< grammar >( __LINE__, __FILE__, true, false );

      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\";", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = 'proto3'; package foo.bar;", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\"; message Person { string name = 1; repeated int32 ids = 2; }", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\"; enum Kind { UNKNOWN = 0; STARTED = 1; }", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\"; service Greeter { rpc Hello (Request) returns (Response); }", result_type::success );

      verify_rule< grammar >( __LINE__, __FILE__, "", result_type::global_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto2\";", result_type::global_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\" message X {}", result_type::global_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "syntax = \"proto3\"; message 1Bad {}", result_type::global_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
