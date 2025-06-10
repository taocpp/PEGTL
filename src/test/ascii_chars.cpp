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
      // verify_analyze< ascii::cr >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::esc >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::ff >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::ht >( __LINE__, __FILE__, true, false );
      // verify_analyze< ascii::lf >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::nul >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::sp >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::vt >( __LINE__, __FILE__, true, false );

      // verify_rule< ascii::cr >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::esc >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::ff >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::ht >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      // verify_rule< ascii::lf >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::nul >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::sp >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::vt >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         // verify_char< ascii::cr >( __LINE__, __FILE__, c, c == 13 );
         verify_char< ascii::esc >( __LINE__, __FILE__, c, c == 27 );
         verify_char< ascii::ff >( __LINE__, __FILE__, c, c == 12 );
         verify_char< ascii::ht >( __LINE__, __FILE__, c, c == 9 );
         // verify_char< ascii::lf >( __LINE__, __FILE__, c, c == 10 );
         verify_char< ascii::nul >( __LINE__, __FILE__, c, c == 0 );
         verify_char< ascii::sp >( __LINE__, __FILE__, c, c == 32 );
         verify_char< ascii::vt >( __LINE__, __FILE__, c, c == 11 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
