// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
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
      verify_analyze< cr >( __LINE__, __FILE__, true, false );
      verify_analyze< esc >( __LINE__, __FILE__, true, false );
      verify_analyze< ff >( __LINE__, __FILE__, true, false );
      verify_analyze< ht >( __LINE__, __FILE__, true, false );
      verify_analyze< lf >( __LINE__, __FILE__, true, false );
      verify_analyze< nul >( __LINE__, __FILE__, true, false );
      verify_analyze< sp >( __LINE__, __FILE__, true, false );
      verify_analyze< vt >( __LINE__, __FILE__, true, false );

      verify_rule< cr >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< esc >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ff >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ht >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< lf >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< nul >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< sp >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< vt >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = -100; i < 200; ++i ) {
         const auto c = char( i );

         verify_char< cr >( __LINE__, __FILE__, c, c == 13 );
         verify_char< esc >( __LINE__, __FILE__, c, c == 27 );
         verify_char< ff >( __LINE__, __FILE__, c, c == 12 );
         verify_char< ht >( __LINE__, __FILE__, c, c == 9 );
         verify_char< lf >( __LINE__, __FILE__, c, c == 10 );
         verify_char< nul >( __LINE__, __FILE__, c, c == 0 );
         verify_char< sp >( __LINE__, __FILE__, c, c == 32 );
         verify_char< vt >( __LINE__, __FILE__, c, c == 11 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
