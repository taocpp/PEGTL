// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< identifier >( __LINE__, __FILE__, true, false );

      verify_rule< identifier >( __LINE__, __FILE__, "_", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_a", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_1", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_123", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_1a", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_a1", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "_fro_bble", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "f_o_o42", result_type::success, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< identifier >( __LINE__, __FILE__, "1", result_type::local_failure, 1 );
      verify_rule< identifier >( __LINE__, __FILE__, " ", result_type::local_failure, 1 );
      verify_rule< identifier >( __LINE__, __FILE__, " _", result_type::local_failure, 2 );
      verify_rule< identifier >( __LINE__, __FILE__, " a", result_type::local_failure, 2 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
