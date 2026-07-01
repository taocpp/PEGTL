// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
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
      verify_meta< range< 'a', 'a' >, internal::one< internal::peek_char, 'a' > >();
      verify_meta< range< 'a', 'f' >, internal::range< internal::peek_char, 'a', 'f' > >();

      verify_ctrl_enabled< range< 'a', 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::one< internal::peek_char, 'a' > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::terminal< internal::one< internal::peek_char, 'a' >, internal::peek_char > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< range< 'a', 'f' > >( __LINE__, __FILE__, "c" );
      verify_ctrl_disabled< internal::range< internal::peek_char, 'a', 'f' > >( __LINE__, __FILE__, "c" );
      verify_ctrl_disabled< internal::terminal< internal::range< internal::peek_char, 'a', 'f' >, internal::peek_char > >( __LINE__, __FILE__, "c" );

      verify_analyze< range< 'a', 'f' > >( __LINE__, __FILE__, true, false );

      verify_rule< range< 'a', 'z' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );

      for( int i = 0; i < 128; ++i ) {
         const auto c = char( i );
         const bool is_range = ( 20 <= c ) && ( c <= 120 );

         verify_char< range< 'a', 'f' > >( __LINE__, __FILE__, c, ( 'a' <= c ) && ( c <= 'f' ) );
         verify_char< range< 20, 120 > >( __LINE__, __FILE__, c, is_range );
      }
      if constexpr( std::is_unsigned_v< char > ) {
         for( int i = 128; i < 256; ++i ) {
            const auto c = char( i );

            verify_char< range< 'a', 'f' > >( __LINE__, __FILE__, c, false );
            verify_char< range< 20, 120 > >( __LINE__, __FILE__, c, false );
         }
      }
      else {  // std::is_signed_v< char >
         for( int i = -128; i < 0; ++i ) {
            const auto c = char( i );

            verify_char< range< 'a', 'f' > >( __LINE__, __FILE__, c, false );
            verify_char< range< 20, 120 > >( __LINE__, __FILE__, c, false );
         }
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
