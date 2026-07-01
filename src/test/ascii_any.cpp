// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< ascii::any7, internal::any< internal::peek_seven > >();
      verify_meta< ascii::any, internal::any< internal::peek_char > >();

      verify_ctrl_enabled< ascii::any7 >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::any< internal::peek_seven > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< ascii::any >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::any< internal::peek_char > >( __LINE__, __FILE__, "a" );

      verify_analyze< ascii::any7 >( __LINE__, __FILE__, true, false );
      verify_analyze< ascii::any >( __LINE__, __FILE__, true, false );

      verify_rule< ascii::any7 >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< ascii::any >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const auto c = char( i );

         verify_char< ascii::any7 >( __LINE__, __FILE__, c, true );
         verify_char< ascii::any >( __LINE__, __FILE__, c, true );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< ascii::any7 >( __LINE__, __FILE__, c, false );
            verify_char< ascii::any >( __LINE__, __FILE__, c, true );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< ascii::any7 >( __LINE__, __FILE__, c, false );
            verify_char< ascii::any >( __LINE__, __FILE__, c, true );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
