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

#include <tao/pegtl/example/ipv6.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using ADDRESS = ipv6::address;
   using GRAMMAR = seq< ipv6::address, eof >;

   void unit_test()
   {
      verify_analyze< GRAMMAR >( __LINE__, __FILE__, true, false );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "::", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "::1", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1::", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1::2", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7:8", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "::1:2:3:4:5:6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1::2:3:4:5:6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2::3:4:5:6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3::4:5:6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4::5:6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5::6:7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6::7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7::", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "2001:db8::7", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "::192.0.2.1", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "::ffff:192.0.2.128", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:192.0.2.1", result_type::success );

      verify_rule< ADDRESS >( __LINE__, __FILE__, "::suffix", result_type::success, 6 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "::1%eth0", result_type::success, 5 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "1::/64", result_type::success, 3 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "2001:db8::7/path", result_type::success, 5 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "::ffff:192.0.2.128:80", result_type::success, 3 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, ":", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, ":::", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "12345::", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "gggg::", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7:", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, ":1:2:3:4:5:6:7", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7:8:", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7:8:9", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1::2::3", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6::7:8", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7::8", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:7:192.0.2.1", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1:2:3:4:5:6:192.0.2.999", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
