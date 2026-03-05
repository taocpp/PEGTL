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
   struct from_chars_rule_auto
   {
      using rule_t = from_chars_rule_auto;
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
            in.template consume< from_chars_rule_auto >( done );
            return true;
         }
         return false;
      }
   };

   template< std::uint8_t Base, typename Integral >
   struct from_chars_rule_type
   {
      using rule_t = from_chars_rule_type;
      using subs_t = empty_list;

      static_assert( std::is_integral_v< Integral > );

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         Integral dummy;
         return match< A, M, Action, Control >( in, dummy );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, Integral& out )
      {
         const std::size_t size = in.size( 3 + ( sizeof( Integral ) * 8 ) );

         if( const std::size_t done = from_chars_impl( in.current(), in.current( size ), out, int( Base ) ) ) {
            in.template consume< from_chars_rule_type >( done );
            return true;
         }
         return false;
      }
   };

   template< std::uint8_t Base >
   inline constexpr bool enable_control< from_chars_rule_auto< Base > > = false;

   template< std::uint8_t Base, typename Integral >
   inline constexpr bool enable_control< from_chars_rule_type< Base, Integral > > = false;

   template< std::uint8_t Base >
   struct from_chars_action_auto
   {
      template< typename ActionInput, typename Integral >
      [[nodiscard]] static bool apply( const ActionInput& in, Integral& out ) noexcept
      {
         static_assert( std::is_integral_v< Integral > );
         return ( !in.empty() ) && ( internal::from_chars_impl( in.begin(), in.end(), out, int( Base ) ) == in.size() );
      }
   };

   template< std::uint8_t Base, typename Integral >
   struct from_chars_action_type
   {
      static_assert( std::is_integral_v< Integral > );

      template< typename ActionInput >
      [[nodiscard]] static bool apply( const ActionInput& in ) noexcept
      {
         Integral dummy;
         return apply( in, dummy );
      }

      template< typename ActionInput >
      [[nodiscard]] static bool apply( const ActionInput& in, Integral& out ) noexcept
      {
         return ( !in.empty() ) && ( internal::from_chars_impl( in.begin(), in.end(), out, int( Base ) ) == in.size() );
      }
   };

   template< std::uint8_t Base >
   struct from_chars_auto
      : from_chars_rule_auto< Base >,
        from_chars_action_auto< Base >
   {
      static_assert( Base != 1 );
   };

   template< std::uint8_t Base, typename Integral >
   struct from_chars_type
      : from_chars_rule_type< Base, Integral >,
        from_chars_action_type< Base, Integral >
   {
      static_assert( Base != 1 );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
