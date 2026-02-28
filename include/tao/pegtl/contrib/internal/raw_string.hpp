// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_RAW_STRING_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_RAW_STRING_HPP

#include <cstddef>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../eol_exclude_tag.hpp"
#include "../../eol_unknown_tag.hpp"
#include "../../rewind_mode.hpp"
#include "../../type_list.hpp"

#include "../../internal/eol.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char Open, char Marker >
   struct raw_string_open
   {
      using rule_t = raw_string_open;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, std::size_t& marker_size, States&&... /*unused*/ ) noexcept( noexcept( in.size( 42 ) ) )
      {
         if( in.empty() || ( in.peek_char( 0 ) != Open ) ) {
            return false;
         }
         for( std::size_t i = 1; i < in.size( i + 1 ); ++i ) {
            switch( const auto c = in.peek_char( i ) ) {
               case Open:
                  marker_size = i + 1;
                  in.template consume< eol_exclude_tag >( marker_size );
                  (void)Control< eol >::template match< A, M, Action, Control >( in );
                  return true;
               case Marker:
                  break;
               default:
                  return false;
            }
         }
         return false;
      }
   };

   template< char Open, char Marker >
   inline constexpr bool enable_control< raw_string_open< Open, Marker > > = false;

   template< char Marker, char Close >
   struct at_raw_string_close
   {
      using rule_t = at_raw_string_close;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, const std::size_t& marker_size, States&&... /*unused*/ ) noexcept( noexcept( in.size( 42 ) ) )
      {
         if( in.size( marker_size ) < marker_size ) {
            return false;
         }
         if( in.peek_char( 0 ) != Close ) {
            return false;
         }
         if( in.peek_char( marker_size - 1 ) != Close ) {
            return false;
         }
         for( std::size_t i = 0; i < ( marker_size - 2 ); ++i ) {
            if( in.peek_char( i + 1 ) != Marker ) {
               return false;
            }
         }
         return true;
      }
   };

   template< char Marker, char Close >
   inline constexpr bool enable_control< at_raw_string_close< Marker, Close > > = false;

   template< char Open, char Marker, char Close, typename Content >
   struct raw_string
   {
      using rule_t = raw_string;
      using subs_t = type_list< raw_string_open< Open, Marker >, Content >;

      struct content
         : Content
      {};

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         std::size_t marker_size;
         if( Control< raw_string_open< Open, Marker > >::template match< A, M, Action, Control >( in, marker_size ) ) {
            if( Control< content >::template match< A, M, Action, Control >( in, marker_size, st... ) ) {
               in.template consume< eol_exclude_tag >( marker_size );
               return true;
            }
         }
         return false;
      }
   };

   template< char Open, char Marker, char Close, typename Content >
   inline constexpr bool enable_control< raw_string< Open, Marker, Close, Content > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
