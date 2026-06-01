// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/example/escaped.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using c_test_rule = seq< one< '\\' >, sor< c_escaped_char, hex_escaped_byte, short_escaped_unicode, long_escaped_unicode >, eof >;
   using json_test_char = seq< star< json_character >, eof >;

   void unit_test()
   {
      verify_analyze< c_test_rule >( __LINE__, __FILE__, true, false );
      verify_analyze< json_test_char >( __LINE__, __FILE__, false, false );

      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\n", result_type::success );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\0", result_type::success );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\x7f", result_type::success );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\u00ff", result_type::success );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\U0010ffff", result_type::success );

      verify_rule< c_test_rule >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\q", result_type::local_failure );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\x7", result_type::local_failure );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\u12", result_type::local_failure );
      verify_rule< c_test_rule >( __LINE__, __FILE__, "\\U0011000", result_type::local_failure );

      verify_rule< json_test_char >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< json_test_char >( __LINE__, __FILE__, "plain text", result_type::success );
      verify_rule< json_test_char >( __LINE__, __FILE__, "\\n", result_type::success );
      verify_rule< json_test_char >( __LINE__, __FILE__, "\\/", result_type::success );
      verify_rule< json_test_char >( __LINE__, __FILE__, "\\u002f", result_type::success );
      verify_rule< json_test_char >( __LINE__, __FILE__, "\\uD834\\uDD1E", result_type::success );

      verify_rule< json_test_char >( __LINE__, __FILE__, "\\a", result_type::local_failure );
      verify_rule< json_test_char >( __LINE__, __FILE__, "\\u12", result_type::local_failure );
      verify_rule< json_test_char >( __LINE__, __FILE__, std::string( 1, char( 0x1f ) ), result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
