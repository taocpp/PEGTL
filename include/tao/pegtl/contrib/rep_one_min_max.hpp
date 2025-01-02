// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_REP_ONE_MIN_MAX_HPP
#define TAO_PEGTL_CONTRIB_REP_ONE_MIN_MAX_HPP

#include <algorithm>
#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "../debug/analyze_traits.hpp"
#include "../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< unsigned Min, unsigned Max, char C >
      struct rep_one_min_max
      {
         using rule_t = rep_one_min_max;
         using subs_t = empty_list;

         static_assert( Min <= Max );

         [[nodiscard]] static constexpr bool test( const char c ) noexcept
         {
            return C == c;
         }

         template< typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in )
         {
            const auto size = in.size( Max + 1 );
            if( size < Min ) {
               return false;
            }
            std::size_t i = 0;
            while( ( i < size ) && ( in.peek_char( i ) == C ) ) {
               ++i;
            }
            if( ( Min <= i ) && ( i <= Max ) ) {
               in.template consume< rep_one_min_max >( i );
               return true;
            }
            return false;
         }
      };

      template< unsigned Max, char C >
      struct rep_one_min_max< 0, Max, C >
      {
         using rule_t = rep_one_min_max;
         using subs_t = empty_list;

         static_assert( 0 < Max );

         [[nodiscard]] static constexpr bool test( const char c ) noexcept
         {
            return C == c;
         }

         template< typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in )
         {
            const auto size = in.size( Max + 1 );
            std::size_t i = 0;
            while( ( i < size ) && ( in.peek_char( i ) == C ) ) {
               ++i;
            }
            if( i <= Max ) {
               in.template consume< rep_one_min_max >( i );
               return true;
            }
            return false;
         }
      };

      template< unsigned Min, unsigned Max, char C >
      inline constexpr bool enable_control< rep_one_min_max< Min, Max, C > > = false;

   }  // namespace internal

   inline namespace ascii
   {
      template< unsigned Min, unsigned Max, char C >
      struct rep_one_min_max
         : internal::rep_one_min_max< Min, Max, C >
      {};

   }  // namespace ascii

   template< typename Name, unsigned Min, unsigned Max, char C >
   struct analyze_traits< Name, internal::rep_one_min_max< Min, Max, C > >
      : std::conditional_t< ( Min != 0 ), analyze_any_traits<>, analyze_opt_traits<> >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
