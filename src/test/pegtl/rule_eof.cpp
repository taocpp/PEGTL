// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
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
      verify_meta< eof, internal::eof >();

      verify_analyze< eof >( __LINE__, __FILE__, false, false );

      verify_rule< eof >( __LINE__, __FILE__, "", result_type::success, 0 );

      for( char i = 1; i < 127; ++i ) {
         verify_char< eof >( __LINE__, __FILE__, i, result_type::local_failure );
      }
      verify_rule< eof >( __LINE__, __FILE__, "abcdefghijklmn", result_type::local_failure, 14 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
