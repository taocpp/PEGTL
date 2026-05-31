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

#include <tao/pegtl/example/semver2.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using SEMVER = semver2::valid_semver;
   using GRAMMAR = must< semver2::valid_semver, eof >;

   void unit_test()
   {
      verify_analyze< SEMVER >( __LINE__, __FILE__, true, false );
      verify_analyze< GRAMMAR >( __LINE__, __FILE__, true, false );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "0.0.0", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "10.20.30", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-alpha", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-alpha.1", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-0.3.7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-x.7.z.92", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-x-y-z.--", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-alpha+001", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0+20130313144700", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-beta+exp.sha.5114f85", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-0A.is.legal", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0-01a", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.0.0+21AF26D3----117B344092BD", result_type::success );

      verify_rule< SEMVER >( __LINE__, __FILE__, "1.2.3 garbage", result_type::success, 8 );
      verify_rule< SEMVER >( __LINE__, __FILE__, "1.2.3.4", result_type::success, 2 );
      verify_rule< SEMVER >( __LINE__, __FILE__, "1.2.3-alpha_beta", result_type::success, 5 );
      verify_rule< SEMVER >( __LINE__, __FILE__, "1.2.3+build?meta", result_type::success, 5 );
      verify_rule< SEMVER >( __LINE__, __FILE__, "1.2.3+build/meta", result_type::success, 5 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3.4", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "01.2.3", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.02.3", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.03", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3+", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-alpha.", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-alpha..beta", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3+build.", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3+build..meta", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-0123", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-01.2", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3-alpha_beta", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "v1.2.3", result_type::global_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3+build?meta", result_type::global_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
