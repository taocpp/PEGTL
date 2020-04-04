// Copyright (c) 2017-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
#include "verify_rule.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         verify_analyze< bol >( __LINE__, __FILE__, false, false );

         verify_only< bol >( __LINE__, __FILE__, "", result_type::success, 0 );

         for( char i = 1; i < 127; ++i ) {
            const char s[] = { i, 0 };  // NOLINT
            verify_only< bol >( __LINE__, __FILE__, s, result_type::success, 1 );
         }
         verify_only< seq< alpha, bol > >( __LINE__, __FILE__, "a", result_type::local_failure, 1 );
         verify_only< seq< alpha, bol > >( __LINE__, __FILE__, "ab", result_type::local_failure, 2 );
         verify_only< seq< alpha, bol, alpha > >( __LINE__, __FILE__, "ab", result_type::local_failure, 2 );
         verify_only< seq< alpha, eol, bol, alpha, eof > >( __LINE__, __FILE__, "a\nb", result_type::success, 0 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
