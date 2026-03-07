// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_CHARCONV_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_CHARCONV_HPP

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/extra/internal/charconv.hpp"
#else

#include <charconv>
#include <cstdint>
#include <system_error>
#include <type_traits>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../parse_error.hpp"
#include "../../rewind_mode.hpp"
#include "../../type_list.hpp"

#include "../../debug/analyze_traits.hpp"

#include "../../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input, typename Integral >
   [[nodiscard]] std::size_t from_chars_impl( const Input& in, const std::size_t size, Integral& out, const int base )
   {
      const char* const begin = in.current();
      const auto result = std::from_chars( begin, begin + size, out, base );
      switch( result.ec ) {
         case std::errc::invalid_argument:
            return 0;
         case std::errc::result_out_of_range:
            throw_parse_error( "integer overflow", in );
         default:
            return result.ptr - begin;
      }
   }

   template< std::uint8_t Base, typename Integral >
   struct from_chars_rule
   {
      using rule_t = from_chars_rule;
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

         if( const std::size_t done = from_chars_impl( in, in.size( size ), out, int( Base ) ) ) {
            in.template consume< from_chars_rule >( done );
            return true;
         }
         return false;
      }
   };

   template< std::uint8_t Base >
   struct from_chars_rule< Base, void >
   {
      using rule_t = from_chars_rule;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename Integral >
      [[nodiscard]] static auto match( ParseInput& in, Integral& out ) -> std::enable_if_t< std::is_integral_v< Integral >, bool >
      {
         const std::size_t size = in.size( 3 + ( sizeof( Integral ) * 8 ) );

         if( const std::size_t done = from_chars_impl( in, in.size( size ), out, int( Base ) ) ) {
            in.template consume< from_chars_rule >( done );
            return true;
         }
         return false;
      }
   };

   template< std::uint8_t Base, typename Integral >
   inline constexpr bool enable_control< from_chars_rule< Base, Integral > > = false;

   template< std::uint8_t Base, typename Integral >
   struct from_chars_action
   {
      static_assert( std::is_integral_v< Integral > );

      template< typename ActionInput >
      [[nodiscard]] static bool apply( const ActionInput& in )
      {
         Integral dummy;
         return apply( in, dummy );
      }

      template< typename ActionInput >
      [[nodiscard]] static bool apply( const ActionInput& in, Integral& out )
      {
         return ( !in.empty() ) && ( from_chars_impl( in, in.size(), out, int( Base ) ) == in.size() );
      }
   };

   template< std::uint8_t Base >
   struct from_chars_action< Base, void >
   {
      template< typename ActionInput, typename Integral >
      [[nodiscard]] static auto apply( const ActionInput& in, Integral& out ) -> std::enable_if_t< std::is_integral_v< Integral >, bool >
      {
         static_assert( std::is_integral_v< Integral > );
         return ( !in.empty() ) && ( from_chars_impl( in, in.size(), out, int( Base ) ) == in.size() );
      }
   };

   template< std::uint8_t Base, typename Integral >
   struct from_chars_combo
      : from_chars_rule< Base, Integral >,
        from_chars_action< Base, Integral >
   {
      static_assert( Base != 1 );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
