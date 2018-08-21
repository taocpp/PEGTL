// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/contrib/if_then.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         // clang-format off
         using grammar =
            if_then< one< 'a' >, one< 'b' >, one< 'c' > >::
            else_if_then< one< 'a' >, one< 'b' > >::
            else_then< one< 'c' > >;

         verify_rule< grammar >( __LINE__, __FILE__, "abc", result_type::SUCCESS, 0 );
         verify_rule< grammar >( __LINE__, __FILE__, "abcd", result_type::SUCCESS, 1 );
         verify_rule< grammar >( __LINE__, __FILE__, "ab", result_type::LOCAL_FAILURE, 2 );
         verify_rule< grammar >( __LINE__, __FILE__, "c", result_type::SUCCESS, 0 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
