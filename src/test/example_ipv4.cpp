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

#include <tao/pegtl/example/ipv4.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using ADDRESS = ipv4::address;
   using GRAMMAR = seq< ipv4::address, eof >;

   void unit_test()
   {
      verify_analyze< GRAMMAR >( __LINE__, __FILE__, true, false );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "0.0.0.0", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "127.0.0.1", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "192.168.0.1", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "255.255.255.255", result_type::success );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "001.002.003.004", result_type::success );

      verify_rule< ADDRESS >( __LINE__, __FILE__, "192.0.2.1:80", result_type::success, 3 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "127.0.0.1/path", result_type::success, 5 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "10.0.0.1 ", result_type::success, 1 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "255.255.255.255suffix", result_type::success, 6 );
      verify_rule< ADDRESS >( __LINE__, __FILE__, "255.255.255.255.0", result_type::success, 2 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "127.1", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "127.0.1", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "030052000001", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "0xc0000201", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "256.0.0.1", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3.256", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "-1.2.3.4", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "+1.2.3.4", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1..2.3", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3.", result_type::local_failure );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1.2.3.4.5", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
