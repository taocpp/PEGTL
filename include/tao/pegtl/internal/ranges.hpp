// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_RANGES_HPP
#define TAO_PEGTL_INTERNAL_RANGES_HPP

#include "../config.hpp"

#include "bump_help.hpp"
#include "enable_control.hpp"
#include "failure.hpp"
#include "one.hpp"
#include "range.hpp"

#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct ranges
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = ranges;
      using subs_t = empty_list;

      [[nodiscard]] static constexpr bool test( const data_t c ) noexcept
      {
         constexpr data_t cs[] = { Cs... };
         for( size_t i = 1; i < sizeof...( Cs ); i += 2 ) {
            if( ( cs[ i - 1 ] <= c ) && ( c <= cs[ i ] ) ) {
               return true;
            }
         }
         if constexpr( sizeof...( Cs ) % 2 == 1 ) {
            return c == cs[ sizeof...( Cs ) - 1 ];
         }
         return false;
      }

      template< int Eol >
      static constexpr bool can_match_eol = test( Eol );

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( test( t.data ) ) {
               bump_help< ranges >( in, t.size );
               return true;
            }
         }
         return false;
      }

   private:
      [[nodiscard]] static constexpr bool check_ranges() noexcept
      {
         constexpr data_t cs[] = { Cs... };
         for( size_t i = 1; i < sizeof...( Cs ); i += 2 ) {
            if( cs[ i - 1 ] > cs[ i ] ) {
               return false;
            }
         }
         return true;
      }

      static_assert( check_ranges(), "invalid range detected" );
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct ranges< Peek, Lo, Hi >
      : range< result_on_found::success, Peek, Lo, Hi >
   {};

   template< typename Peek, typename Peek::data_t C >
   struct ranges< Peek, C >
      : one< result_on_found::success, Peek, C >
   {};

   template< typename Peek >
   struct ranges< Peek >
      : failure
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< ranges< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
