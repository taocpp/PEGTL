// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_CHARCONV_HPP
#define TAO_PEGTL_CONTRIB_CHARCONV_HPP

#include <cassert>
#include <charconv>
#include <cstdint>
#include <system_error>
#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../eol_exclude_tag.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "../debug/analyze_traits.hpp"

#include "../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< std::uint8_t Base >
      struct charconv
      {
         using rule_t = charconv;
         using subs_t = empty_list;

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename Integral >
         [[nodiscard]] static bool match( ParseInput& in, Integral& out )
         {
            static_assert( Base != 1 );
            static_assert( std::is_integral_v< Integral > );

            const std::size_t size = in.size( 2 + ( sizeof( Integral ) * 8 ) );

            if( size > 0 ) {
               const auto result = std::from_chars( in.current(), in.current( size ), out, int( Base ) );
               switch( result.ec ) {
                  case std::errc::invalid_argument:
                     return false;
                  case std::errc::result_out_of_range:
                     return false;  // throw?
                  default:
                     in.template consume< eol_exclude_tag >( result.ptr - in.current() );
                     return true;
               }
            }
            return false;
         }
      };

      template< std::uint8_t Base >
      inline constexpr bool enable_control< charconv< Base > > = false;

   }  // namespace internal

   struct charconv
      : internal::charconv< 10 >
   {};

   template< typename Name, std::uint8_t Base >
   struct analyze_traits< Name, internal::charconv< Base > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
