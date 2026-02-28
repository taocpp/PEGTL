// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_CHARCONV_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_CHARCONV_HPP

#include <charconv>
#include <cstdint>
#include <system_error>
#include <type_traits>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../rewind_mode.hpp"
#include "../../type_list.hpp"

#include "../../debug/analyze_traits.hpp"

#include "../../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Integral >
   [[nodiscard]] std::size_t from_chars_impl( const char* begin, const char* end, Integral& out, const int base ) noexcept
   {
      const auto result = std::from_chars( begin, end, out, base );
      switch( result.ec ) {
         case std::errc::invalid_argument:
            return 0;
         case std::errc::result_out_of_range:
            return 0;
         default:
            return result.ptr - begin;
      }
   }

   template< std::uint8_t Base >
   struct from_chars_rule
   {
      using rule_t = from_chars_rule;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename Integral >
      [[nodiscard]] static bool match( ParseInput& in, Integral& out )
      {
         static_assert( std::is_integral_v< Integral > );

         const std::size_t size = in.size( 3 + ( sizeof( Integral ) * 8 ) );

         if( const std::size_t done = from_chars_impl( in.current(), in.current( size ), out, int( Base ) ) ) {
            in.template consume< from_chars_rule >( done );
            return true;
         }
         return false;
      }
   };

   template< std::uint8_t Base >
   inline constexpr bool enable_control< from_chars_rule< Base > > = false;

   template< std::uint8_t Base >
   struct from_chars_action
   {
      template< typename ActionInput, typename Integral >
      [[nodiscard]] static bool apply( const ActionInput& in, Integral& out ) noexcept
      {
         static_assert( std::is_integral_v< Integral > );
         return ( !in.empty() ) && ( internal::from_chars_impl( in.begin(), in.end(), out, int( Base ) ) == in.size() );
      }
   };

   template< std::uint8_t Base >
   struct from_chars
      : from_chars_rule< Base >,
        from_chars_action< Base >
   {
      static_assert( Base != 1 );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
