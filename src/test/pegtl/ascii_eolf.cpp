// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< eolf >( __LINE__, __FILE__, false, false );

      verify_rule< eolf >( __LINE__, __FILE__, "", result_type::success, 0 );

      for( char i = 1; i < 127; ++i ) {
         verify_char< eolf >( __LINE__, __FILE__, i, ( i == '\n' ) ? result_type::success : result_type::local_failure );
      }
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\r", result_type::local_failure, 1 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\n", result_type::success, 0 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\r\n", result_type::local_failure, 2 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\n\r", result_type::success, 1 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\n\r\n", result_type::success, 2 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\n\r\r", result_type::success, 2 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\na", result_type::success, 1 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\ra", result_type::local_failure, 2 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\r\na", result_type::local_failure, 3 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\r\n\r", result_type::local_failure, 3 );
      verify_rule< eolf, scan::lf >( __LINE__, __FILE__, "\r\n\n", result_type::local_failure, 3 );

      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\r", result_type::success, 0 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\n", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\r\n", result_type::success, 1 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\n\r", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\n\r\n", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\n\r\r", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\na", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\ra", result_type::success, 1 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\r\na", result_type::success, 2 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\r\n\r", result_type::success, 2 );
      verify_rule< eolf, ascii::cr >( __LINE__, __FILE__, "\r\n\n", result_type::success, 2 );

      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\r", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\n", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\r\n", result_type::success, 0 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\n\r", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\n\r\n", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\n\r\r", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\na", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\ra", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\r\na", result_type::success, 1 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\r\n\r", result_type::success, 1 );
      verify_rule< eolf, ascii::crlf >( __LINE__, __FILE__, "\r\n\n", result_type::success, 1 );

      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\r", result_type::local_failure, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\n", result_type::success, 0 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\r\n", result_type::success, 0 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\n\r", result_type::success, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\n\r\n", result_type::success, 2 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\n\r\r", result_type::success, 2 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\na", result_type::success, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\ra", result_type::local_failure, 2 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\r\na", result_type::success, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\r\n\r", result_type::success, 1 );
      verify_rule< eolf, scan::lf_crlf >( __LINE__, __FILE__, "\r\n\n", result_type::success, 1 );

      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\r", result_type::success, 0 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\n", result_type::local_failure, 1 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\r\n", result_type::success, 0 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\n\r", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\n\r\n", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\n\r\r", result_type::local_failure, 3 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\na", result_type::local_failure, 2 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\ra", result_type::success, 1 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\r\na", result_type::success, 1 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\r\n\r", result_type::success, 1 );
      verify_rule< eolf, ascii::cr_crlf >( __LINE__, __FILE__, "\r\n\n", result_type::success, 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
