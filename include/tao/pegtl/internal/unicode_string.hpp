// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNICODE_STRING_HPP
#define TAO_PEGTL_INTERNAL_UNICODE_STRING_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_string.hpp"
#include "dependent_false.hpp"
#include "enable_control.hpp"
#include "one.hpp"
#include "peek_unicode.hpp"
#include "peek_utf16.hpp"
#include "peek_utf32.hpp"
#include "success.hpp"
#include "unicode_to_utf8_string.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char32_t... Cs >
   struct unicode_string
   {
      using rule_t = unicode_string;
      using subs_t = empty_list;

      static_assert( sizeof...( Cs ) > 0 );

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... /*unused*/ ) noexcept( noexcept( in.size( 1 ) ) )
      {
         static constexpr std::size_t size = sizeof( typename ParseInput::data_t );

         if constexpr( size == 1 ) {
            return unicode_to_utf8_string_t< ascii_string, Cs... >::match( in );
         }
         else if constexpr( size == 2 ) {
            return seq< one< peek_utf16, Cs >... >::template match< A, M, Action, Control >( in );
         }
         else if constexpr( size == 4 ) {
            return seq< one< peek_utf32, Cs >... >::template match< A, M, Action, Control >( in );
         }
         else {
            static_assert( dependent_false< ParseInput > );
         }
      }
   };

   template< char32_t C >
   struct unicode_string< C >
      : one< peek_unicode, C >
   {};

   template<>
   struct unicode_string<>
      : success
   {};

   template< char... Cs >
   inline constexpr bool enable_control< unicode_string< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
